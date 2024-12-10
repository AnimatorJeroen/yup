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

/*
  ==============================================================================

  BEGIN_JUCE_MODULE_DECLARATION

    ID:                 rive_renderer
    vendor:             rive
    version:            1.0
    name:               Rive Renderer.
    description:        The Rive Renderer is a vector and raster graphics renderer custom-built for Rive content, for animation, and for runtime.
    website:            https://github.com/rive-app/rive-runtime
    license:            MIT

    dependencies:       rive glad
    searchpaths:        include source
    osxFrameworks:      Metal QuartzCore
    defines:            WITH_RIVE_TEXT=1
    iosDefines:         RIVE_IOS=1
    linuxDefines:       RIVE_DESKTOP_GL=1
    wasmDefines:        RIVE_WEBGL=1
    androidDefines:     RIVE_ANDROID=1
    androidLibs:        EGL GLESv3
    enableARC:          1

  END_JUCE_MODULE_DECLARATION

  ==============================================================================
*/

#pragma once

//==============================================================================
/** Config: YUP_RIVE_USE_METAL
    Enables the use of the Metal renderer on macOS (the default is enabled).
*/
#ifndef YUP_RIVE_USE_METAL
#define YUP_RIVE_USE_METAL 1
#endif

/** Config: YUP_RIVE_USE_D3D
    Enables the use of the Direct3D renderer on Windows (the default is enabled).
*/
#ifndef YUP_RIVE_USE_D3D
#define YUP_RIVE_USE_D3D 1
#endif

/** Config: YUP_RIVE_USE_OPENGL
    Enables the use of the OpenGL renderer on platform that support it but where it is not used by default (in
    the specific case macOS and Windows).

    You will need to link to the specific OpenGL framework on macOS when building your application with this
    flag set: add "-framework OpenGL" to link flags.
*/
#ifndef YUP_RIVE_USE_OPENGL
#define YUP_RIVE_USE_OPENGL 0
#endif

/** Config: YUP_RIVE_USE_DAWN
    Enables the use of the Dawn renderer on platform that support it.
*/
#ifndef YUP_RIVE_USE_DAWN
#define YUP_RIVE_USE_DAWN 0
#endif

//==============================================================================
/** Config: YUP_RIVE_OPENGL_MAJOR
    Enables a speficic OpenGL major version. Must be at least 4.
*/
#ifndef YUP_RIVE_OPENGL_MAJOR
#define YUP_RIVE_OPENGL_MAJOR 4
#endif

/** Config: YUP_RIVE_OPENGL_MINOR
    Enables a speficic OpenGL minor version. Must be at least 2.
*/
#ifndef YUP_RIVE_OPENGL_MINOR
#define YUP_RIVE_OPENGL_MINOR 2
#endif

//==============================================================================

#if YUP_RIVE_USE_OPENGL
#if __APPLE__ && !YUP_RIVE_USE_METAL && !YUP_RIVE_USE_DAWN
#error Must select at least one between YUP_RIVE_USE_METAL, YUP_RIVE_USE_OPENGL or YUP_RIVE_USE_DAWN
#elif _WIN32 && !YUP_RIVE_USE_D3D && !YUP_RIVE_USE_DAWN
#error Must select at least one between YUP_RIVE_USE_D3D, YUP_RIVE_USE_OPENGL or YUP_RIVE_USE_DAWN
#endif

#if !defined(RIVE_DESKTOP_GL) && !defined(RIVE_WEBGL)
#define RIVE_DESKTOP_GL 1
#endif
#endif

#if YUP_RIVE_USE_DAWN
#if !defined(RIVE_DAWN)
#define RIVE_DAWN 1
#endif
#endif
