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

#include "zlib.h"

#include <stdlib.h>
#include <stdio.h>

#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable: 4309)
#pragma warning (disable: 4305)
#pragma warning (disable: 4365)
#pragma warning (disable: 6385)
#pragma warning (disable: 6326)
#pragma warning (disable: 6340)
#else
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wdeprecated-register"
#pragma clang diagnostic ignored "-Wswitch-enum"
#pragma clang diagnostic ignored "-Wswitch-default"
#pragma clang diagnostic ignored "-Wredundant-decls"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wcomma"
#endif

#include "src/adler32.c"
#include "src/compress.c"
#undef DO1
#undef DO8
#include "src/crc32.c"
#include "src/deflate.c"
#include "src/inffast.c"
#undef PULLBYTE
#undef LOAD
#undef RESTORE
#undef INITBITS
#undef NEEDBITS
#undef DROPBITS
#undef BYTEBITS
#include "src/inflate.c"
#include "src/inftrees.c"
#include "src/trees.c"
#include "src/zutil.c"

#if _MSC_VER
#pragma warning (pop)
#else
#pragma clang diagnostic pop
#endif
