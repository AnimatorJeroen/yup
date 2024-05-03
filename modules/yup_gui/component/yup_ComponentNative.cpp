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

namespace juce
{

//==============================================================================

ComponentNative::ComponentNative (Component& newComponent)
    : component (newComponent)
{
}

ComponentNative::~ComponentNative ()
{
}

//==============================================================================

void ComponentNative::handlePaint (Graphics& g, float frameRate)
{
    component.internalPaint (g, frameRate);
}

//==============================================================================

void ComponentNative::handleMouseMove (const MouseEvent& event)
{
    component.internalMouseMove (event);
}

void ComponentNative::handleMouseDrag (const MouseEvent& event)
{
    component.internalMouseDrag (event);
}

void ComponentNative::handleMouseDown (const MouseEvent& event)
{
    component.internalMouseDown (event);
}

void ComponentNative::handleMouseUp (const MouseEvent& event)
{
    component.internalMouseUp (event);
}

//==============================================================================

void ComponentNative::handleKeyDown (const KeyPress& keys, double x, double y)
{
    component.internalKeyDown (keys, x, y);
}

void ComponentNative::handleKeyUp (const KeyPress& keys, double x, double y)
{
    component.internalKeyUp (keys, x, y);
}

//==============================================================================

void ComponentNative::handleResized (int width, int height)
{
    component.internalResized (width, height);
}

void ComponentNative::handleUserTriedToCloseWindow()
{
    component.internalUserTriedToCloseWindow();
}

} // namespace juce
