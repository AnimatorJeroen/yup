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
*/

namespace yup
{

//==============================================================================

TextButton::TextButton (StringRef componentID, const Font& font)
    : Button (componentID)
    , font (font)
{
}

void TextButton::paintButton (Graphics& g, bool isButtonOver, bool isButtonDown)
{
    auto bounds = getLocalBounds().reduced (proportionOfWidth (0.01f));
    const auto center = bounds.getCenter();

    Path backgroundPath;
    //backgroundPath.clear();
    backgroundPath.addRoundedRectangle (bounds.reduced (proportionOfWidth (0.045f)), 10.0f, 10.0f, 10.0f, 10.0f);
    g.setFillColor (isButtonDown ? Color (0xff000000) : Color (0xffffffff));
    g.fillPath (backgroundPath);

    StyledText text;
    //text.clear();
    text.appendText (font, bounds.getHeight() * 0.25f, bounds.getHeight(), getComponentID().toRawUTF8());
    text.layout (bounds.reduced (0.0f, 10.0f), yup::StyledText::center);

    g.setStrokeColor (isButtonDown ? Color (0xffffffff) : Color (0xff000000));
    g.strokeFittedText (text, {});
}

} // namespace yup
