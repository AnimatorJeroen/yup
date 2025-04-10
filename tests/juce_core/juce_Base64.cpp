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

#include <gtest/gtest.h>

#include <juce_core/juce_core.h>

using namespace juce;

TEST (Base64Tests, Base64EncodingDecoding)
{
    Random random;

    auto createRandomData = [&]
    {
        MemoryOutputStream m;

        for (int i = random.nextInt (400); --i >= 0;)
            m.writeByte (static_cast<char> (random.nextInt (256)));

        return m.getMemoryBlock();
    };

    for (int i = 1000; --i >= 0;)
    {
        auto original = createRandomData();
        auto asBase64 = Base64::toBase64 (original.getData(), original.getSize());

        MemoryOutputStream out;
        EXPECT_TRUE (Base64::convertFromBase64 (out, asBase64));

        auto result = out.getMemoryBlock();
        EXPECT_EQ (result, original);
    }
}
