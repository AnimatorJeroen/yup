/*
  ==============================================================================

   This file is part of the YUP library.
   Copyright (c) 2024 - kunitoki@gmail.com

   YUP is an open source library subject to open-source licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   YUP IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2022 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

struct JSONParser
{
    JSONParser (String::CharPointerType text)
        : startLocation (text)
        , currentLocation (text)
    {
    }

    String::CharPointerType startLocation, currentLocation;

    struct ErrorException
    {
        String message;
        int line = 1, column = 1;

        String getDescription() const { return String (line) + ":" + String (column) + ": error: " + message; }

        Result getResult() const { return Result::fail (getDescription()); }
    };

    [[noreturn]] void throwError (juce::String message, String::CharPointerType location)
    {
        ErrorException e;
        e.message = std::move (message);

        for (auto i = startLocation; i < location && ! i.isEmpty(); ++i)
        {
            ++e.column;
            if (*i == '\n')
            {
                e.column = 1;
                e.line++;
            }
        }

        throw e;
    }

    void skipWhitespace() { currentLocation = currentLocation.findEndOfWhitespace(); }

    juce_wchar readChar() { return currentLocation.getAndAdvance(); }

    juce_wchar peekChar() const { return *currentLocation; }

    bool matchIf (char c)
    {
        if (peekChar() == (juce_wchar) c)
        {
            ++currentLocation;
            return true;
        }
        return false;
    }

    bool isEOF() const { return peekChar() == 0; }

    bool matchString (const char* t)
    {
        while (*t != 0)
            if (! matchIf (*t++))
                return false;

        return true;
    }

    var parseObjectOrArray()
    {
        skipWhitespace();

        if (matchIf ('{'))
            return parseObject();
        if (matchIf ('['))
            return parseArray();

        if (! isEOF())
            throwError ("Expected '{' or '['", currentLocation);

        return {};
    }

    String parseString (const juce_wchar quoteChar)
    {
        MemoryOutputStream buffer (256);

        for (;;)
        {
            auto c = readChar();

            if (c == quoteChar)
                break;

            if (c == '\\')
            {
                auto errorLocation = currentLocation;
                c = readChar();

                switch (c)
                {
                    case '"':
                    case '\'':
                    case '\\':
                    case '/':
                        break;

                    case 'a':
                        c = '\a';
                        break;
                    case 'b':
                        c = '\b';
                        break;
                    case 'f':
                        c = '\f';
                        break;
                    case 'n':
                        c = '\n';
                        break;
                    case 'r':
                        c = '\r';
                        break;
                    case 't':
                        c = '\t';
                        break;

                    case 'u':
                    {
                        c = 0;

                        for (int i = 4; --i >= 0;)
                        {
                            auto digitValue = CharacterFunctions::getHexDigitValue (readChar());

                            if (digitValue < 0)
                                throwError ("Syntax error in unicode escape sequence", errorLocation);

                            c = (juce_wchar) ((c << 4) + static_cast<juce_wchar> (digitValue));
                        }

                        break;
                    }

                    default:
                        break;
                }
            }

            if (c == 0)
                throwError ("Unexpected EOF in string constant", currentLocation);

            buffer.appendUTF8Char (c);
        }

        return buffer.toUTF8();
    }

    var parseAny()
    {
        skipWhitespace();
        auto originalLocation = currentLocation;

        switch (readChar())
        {
            case '{':
                return parseObject();
            case '[':
                return parseArray();
            case '"':
                return parseString ('"');
            case '\'':
                return parseString ('\'');

            case '-':
                skipWhitespace();
                return parseNumber (true);

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                currentLocation = originalLocation;
                return parseNumber (false);

            case 't': // "true"
                if (matchString ("rue"))
                    return var (true);

                break;

            case 'f': // "false"
                if (matchString ("alse"))
                    return var (false);

                break;

            case 'n': // "null"
                if (matchString ("ull"))
                    return {};

                break;

            default:
                break;
        }

        throwError ("Syntax error", originalLocation);
    }

    var parseNumber (bool isNegative)
    {
        auto originalPos = currentLocation;

        int64 intValue = readChar() - '0';
        jassert (intValue >= 0 && intValue < 10);

        for (;;)
        {
            auto lastPos = currentLocation;
            auto c = readChar();
            auto digit = ((int) c) - '0';

            if (isPositiveAndBelow (digit, 10))
            {
                intValue = intValue * 10 + digit;
                continue;
            }

            if (c == 'e' || c == 'E' || c == '.')
            {
                currentLocation = originalPos;
                auto asDouble = CharacterFunctions::readDoubleValue (currentLocation);
                return var (isNegative ? -asDouble : asDouble);
            }

            if (CharacterFunctions::isWhitespace (c)
                || c == ',' || c == '}' || c == ']' || c == 0)
            {
                currentLocation = lastPos;
                break;
            }

            throwError ("Syntax error in number", lastPos);
        }

        auto correctedValue = isNegative ? -intValue : intValue;

        return (intValue >> 31) != 0 ? var (correctedValue)
                                     : var ((int) correctedValue);
    }

    var parseObject()
    {
        auto resultObject = new DynamicObject();
        var result (resultObject);
        auto& resultProperties = resultObject->getProperties();
        auto startOfObjectDecl = currentLocation;

        for (;;)
        {
            skipWhitespace();
            auto errorLocation = currentLocation;
            auto c = readChar();

            if (c == '}')
                break;

            if (c == 0)
                throwError ("Unexpected EOF in object declaration", startOfObjectDecl);

            if (c != '"')
                throwError ("Expected a property name in double-quotes", errorLocation);

            errorLocation = currentLocation;
            Identifier propertyName (parseString ('"'));

            if (! propertyName.isValid())
                throwError ("Invalid property name", errorLocation);

            skipWhitespace();
            errorLocation = currentLocation;

            if (readChar() != ':')
                throwError ("Expected ':'", errorLocation);

            resultProperties.set (propertyName, parseAny());

            skipWhitespace();
            if (matchIf (','))
                continue;
            if (matchIf ('}'))
                break;

            throwError ("Expected ',' or '}'", currentLocation);
        }

        return result;
    }

    var parseArray()
    {
        auto result = var (Array<var>());
        auto destArray = result.getArray();
        auto startOfArrayDecl = currentLocation;

        for (;;)
        {
            skipWhitespace();

            if (matchIf (']'))
                break;

            if (isEOF())
                throwError ("Unexpected EOF in array declaration", startOfArrayDecl);

            destArray->add (parseAny());
            skipWhitespace();

            if (matchIf (','))
                continue;
            if (matchIf (']'))
                break;

            throwError ("Expected ',' or ']'", currentLocation);
        }

        return result;
    }
};

//==============================================================================
struct JSONFormatter
{
    static void writeEscapedChar (OutputStream& out, const unsigned short value)
    {
        out << "\\u" << String::toHexString ((int) value).paddedLeft ('0', 4);
    }

    static void writeString (OutputStream& out, String::CharPointerType t)
    {
        for (;;)
        {
            auto c = t.getAndAdvance();

            switch (c)
            {
                case 0:
                    return;

                case '\"':
                    out << "\\\"";
                    break;
                case '\\':
                    out << "\\\\";
                    break;
                case '\a':
                    out << "\\a";
                    break;
                case '\b':
                    out << "\\b";
                    break;
                case '\f':
                    out << "\\f";
                    break;
                case '\t':
                    out << "\\t";
                    break;
                case '\r':
                    out << "\\r";
                    break;
                case '\n':
                    out << "\\n";
                    break;

                default:
                    if (c >= 32 && c < 127)
                    {
                        out << (char) c;
                    }
                    else
                    {
                        if (CharPointer_UTF16::getBytesRequiredFor (c) > 2)
                        {
                            CharPointer_UTF16::CharType chars[2];
                            CharPointer_UTF16 utf16 (chars);
                            utf16.write (c);

                            for (int i = 0; i < 2; ++i)
                                writeEscapedChar (out, (unsigned short) chars[i]);
                        }
                        else
                        {
                            writeEscapedChar (out, (unsigned short) c);
                        }
                    }

                    break;
            }
        }
    }

    static void writeSpaces (OutputStream& out, int numSpaces)
    {
        out.writeRepeatedByte (' ', (size_t) numSpaces);
    }

    static void writeArray (OutputStream& out, const Array<var>& array, const JSON::FormatOptions& format)
    {
        out << '[';

        if (! array.isEmpty())
        {
            if (format.getSpacing() == JSON::Spacing::multiLine)
                out << newLine;

            for (int i = 0; i < array.size(); ++i)
            {
                if (format.getSpacing() == JSON::Spacing::multiLine)
                    writeSpaces (out, format.getIndentLevel() + indentSize);

                JSON::writeToStream (out, array.getReference (i), format.withIndentLevel (format.getIndentLevel() + indentSize));

                if (i < array.size() - 1)
                {
                    out << ",";

                    switch (format.getSpacing())
                    {
                        case JSON::Spacing::none:
                            break;
                        case JSON::Spacing::singleLine:
                            out << ' ';
                            break;
                        case JSON::Spacing::multiLine:
                            out << newLine;
                            break;
                    }
                }
                else if (format.getSpacing() == JSON::Spacing::multiLine)
                    out << newLine;
            }

            if (format.getSpacing() == JSON::Spacing::multiLine)
                writeSpaces (out, format.getIndentLevel());
        }

        out << ']';
    }

    enum
    {
        indentSize = 2
    };
};

void JSON::writeToStream (OutputStream& out, const var& v, const FormatOptions& opt)
{
    if (v.isString())
    {
        out << '"';
        JSONFormatter::writeString (out, v.toString().getCharPointer());
        out << '"';
    }
    else if (v.isVoid())
    {
        out << "null";
    }
    else if (v.isUndefined())
    {
        out << "undefined";
    }
    else if (v.isBool())
    {
        out << (static_cast<bool> (v) ? "true" : "false");
    }
    else if (v.isDouble())
    {
        auto d = static_cast<double> (v);

        if (juce_isfinite (d))
        {
            out << serialiseDouble (d, opt.getMaxDecimalPlaces());
        }
        else
        {
            out << "null";
        }
    }
    else if (v.isArray())
    {
        JSONFormatter::writeArray (out, *v.getArray(), opt);
    }
    else if (v.isObject())
    {
        if (auto* object = v.getDynamicObject())
            object->writeAsJSON (out, opt);
        else
            jassertfalse; // Only DynamicObjects can be converted to JSON!
    }
    else
    {
        // Can't convert these other types of object to JSON!
        jassert (! (v.isMethod() || v.isBinaryData()));

        out << v.toString();
    }
}

String JSON::toString (const var& v, const FormatOptions& opt)
{
    MemoryOutputStream mo { 1024 };
    writeToStream (mo, v, opt);
    return mo.toUTF8();
}

//==============================================================================
var JSON::parse (const String& text)
{
    var result;

    if (parse (text, result))
        return result;

    return {};
}

var JSON::fromString (StringRef text)
{
    try
    {
        return JSONParser (text.text).parseAny();
    }
    catch (const JSONParser::ErrorException&)
    {
    }

    return {};
}

var JSON::parse (InputStream& input)
{
    return parse (input.readEntireStreamAsString());
}

var JSON::parse (const File& file)
{
    return parse (file.loadFileAsString());
}

Result JSON::parse (const String& text, var& result)
{
    try
    {
        result = JSONParser (text.getCharPointer()).parseObjectOrArray();
    }
    catch (const JSONParser::ErrorException& error)
    {
        return error.getResult();
    }

    return Result::ok();
}

String JSON::toString (const var& data, const bool allOnOneLine, int maximumDecimalPlaces)
{
    return toString (data, FormatOptions {}.withSpacing (allOnOneLine ? Spacing::singleLine : Spacing::multiLine).withMaxDecimalPlaces (maximumDecimalPlaces));
}

void JSON::writeToStream (OutputStream& output, const var& data, const bool allOnOneLine, int maximumDecimalPlaces)
{
    writeToStream (output, data, FormatOptions {}.withSpacing (allOnOneLine ? Spacing::singleLine : Spacing::multiLine).withMaxDecimalPlaces (maximumDecimalPlaces));
}

String JSON::escapeString (StringRef s)
{
    MemoryOutputStream mo;
    JSONFormatter::writeString (mo, s.text);
    return mo.toString();
}

Result JSON::parseQuotedString (String::CharPointerType& t, var& result)
{
    try
    {
        JSONParser parser (t);
        auto quote = parser.readChar();

        if (quote != '"' && quote != '\'')
            return Result::fail ("Not a quoted string!");

        result = parser.parseString (quote);
        t = parser.currentLocation;
    }
    catch (const JSONParser::ErrorException& error)
    {
        return error.getResult();
    }

    return Result::ok();
}

} // namespace juce
