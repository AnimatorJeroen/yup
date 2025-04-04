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

#ifdef JUCE_AUDIO_BASICS_H_INCLUDED
/* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
#error "Incorrect use of JUCE cpp file"
#endif

#include "juce_audio_basics.h"

#if JUCE_MINGW && ! defined(alloca)
#define alloca __builtin_alloca
#endif

#if JUCE_USE_SSE_INTRINSICS
#include <emmintrin.h>
#endif

#if JUCE_MAC || JUCE_IOS
#ifndef JUCE_USE_VDSP_FRAMEWORK
#define JUCE_USE_VDSP_FRAMEWORK 1
#endif

#if JUCE_USE_VDSP_FRAMEWORK
#include <Accelerate/Accelerate.h>
#endif

#include "native/juce_AudioWorkgroup_apple.h"

#elif JUCE_USE_VDSP_FRAMEWORK
#undef JUCE_USE_VDSP_FRAMEWORK
#endif

#if JUCE_USE_ARM_NEON
#include <arm_neon.h>
#endif

#include "buffers/juce_AudioDataConverters.cpp"
#include "buffers/juce_FloatVectorOperations.cpp"
#include "buffers/juce_AudioChannelSet.cpp"
#include "buffers/juce_AudioProcessLoadMeasurer.cpp"
#include "utilities/juce_IIRFilter.cpp"
#include "utilities/juce_LagrangeInterpolator.cpp"
#include "utilities/juce_WindowedSincInterpolator.cpp"
#include "utilities/juce_Interpolators.cpp"
#include "utilities/juce_SmoothedValue.cpp"
#include "midi/juce_MidiBuffer.cpp"
#include "midi/juce_MidiFile.cpp"
#include "midi/juce_MidiKeyboardState.cpp"
#include "midi/juce_MidiMessage.cpp"
#include "midi/juce_MidiMessageSequence.cpp"
#include "midi/juce_MidiRPN.cpp"
#include "mpe/juce_MPEValue.cpp"
#include "mpe/juce_MPENote.cpp"
#include "mpe/juce_MPEZoneLayout.cpp"
#include "mpe/juce_MPEInstrument.cpp"
#include "mpe/juce_MPEMessages.cpp"
#include "mpe/juce_MPESynthesiserBase.cpp"
#include "mpe/juce_MPESynthesiserVoice.cpp"
#include "mpe/juce_MPESynthesiser.cpp"
#include "mpe/juce_MPEUtils.cpp"
#include "sources/juce_BufferingAudioSource.cpp"
#include "sources/juce_ChannelRemappingAudioSource.cpp"
#include "sources/juce_IIRFilterAudioSource.cpp"
#include "sources/juce_MemoryAudioSource.cpp"
#include "sources/juce_MixerAudioSource.cpp"
#include "sources/juce_ResamplingAudioSource.cpp"
#include "sources/juce_ReverbAudioSource.cpp"
#include "sources/juce_ToneGeneratorAudioSource.cpp"
#include "sources/juce_PositionableAudioSource.cpp"
#include "synthesisers/juce_Synthesiser.cpp"
#include "audio_play_head/juce_AudioPlayHead.cpp"
#include "midi/juce_MidiDataConcatenator.h"
#include "midi/ump/juce_UMP.h"
#include "midi/ump/juce_UMPUtils.cpp"
#include "midi/ump/juce_UMPView.cpp"
#include "midi/ump/juce_UMPSysEx7.cpp"
#include "midi/ump/juce_UMPMidi1ToMidi2DefaultTranslator.cpp"
#include "midi/ump/juce_UMPIterator.cpp"
#include "utilities/juce_AudioWorkgroup.cpp"

#if JUCE_UNIT_TESTS
#include "utilities/juce_ADSR_test.cpp"
#include "midi/ump/juce_UMP_test.cpp"
#endif
