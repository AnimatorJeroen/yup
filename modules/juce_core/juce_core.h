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

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 juce_core
  vendor:             juce
  version:            7.0.12
  name:               JUCE core classes
  description:        The essential set of basic JUCE classes, as required by all the other JUCE modules. Includes text, container, memory, threading and i/o functionality.
  website:            http://www.juce.com/juce
  license:            ISC
  minimumCppStandard: 17

  dependencies:       zlib
  osxFrameworks:      Cocoa Foundation IOKit Security
  iosFrameworks:      Foundation UIKit
  iosSimFrameworks:   Foundation UIKit
  linuxLibs:          rt dl pthread
  androidLibs:        log android
  androidSearchpaths: {ANDROID_NDK}/sources/android/cpufeatures
  mingwLibs:          ws2_32 uuid wininet version kernel32 user32 wsock32 advapi32 ole32 oleaut32 imm32 comdlg32 shlwapi rpcrt4 winmm

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define JUCE_CORE_H_INCLUDED

//==============================================================================
#ifdef _MSC_VER
#pragma warning(push)
// Disable warnings for long class names, padding, and undefined preprocessor definitions.
#pragma warning(disable : 4251 4786 4668 4820)
#ifdef __INTEL_COMPILER
#pragma warning(disable : 1125)
#endif
#endif

#include "system/juce_TargetPlatform.h"

//==============================================================================
/** Config: JUCE_FORCE_DEBUG

    Normally, JUCE_DEBUG is set to 1 or 0 based on compiler and project settings,
    but if you define this value, you can override this to force it to be true or false.
*/
#ifndef JUCE_FORCE_DEBUG
//#define JUCE_FORCE_DEBUG 0
#endif

//==============================================================================
/** Config: JUCE_LOG_ASSERTIONS

    If this flag is enabled, the jassert and jassertfalse macros will always use Logger::writeToLog()
    to write a message when an assertion happens.

    Enabling it will also leave this turned on in release builds. When it's disabled,
    however, the jassert and jassertfalse macros will not be compiled in a
    release build.

    @see jassert, jassertfalse, Logger
*/
#ifndef JUCE_LOG_ASSERTIONS
#if JUCE_ANDROID
#define JUCE_LOG_ASSERTIONS 1
#else
#define JUCE_LOG_ASSERTIONS 0
#endif
#endif

//==============================================================================
/** Config: JUCE_CHECK_MEMORY_LEAKS

    Enables a memory-leak check for certain objects when the app terminates. See the LeakedObjectDetector
    class and the JUCE_LEAK_DETECTOR macro for more details about enabling leak checking for specific classes.
*/
#if JUCE_DEBUG && ! defined(JUCE_CHECK_MEMORY_LEAKS)
#define JUCE_CHECK_MEMORY_LEAKS 1
#endif

//==============================================================================
/** Config: JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES

    In a Windows build, this can be used to stop the required system libs being
    automatically added to the link stage.
*/
#ifndef JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
#define JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES 0
#endif

/** Config: JUCE_USE_CURL
    Enables http/https support via libcurl (Linux only). Enabling this will add an additional
    run-time dynamic dependency to libcurl.

    If you disable this then https/ssl support will not be available on Linux.
*/
#ifndef JUCE_USE_CURL
#define JUCE_USE_CURL 1
#endif

/** Config: JUCE_LOAD_CURL_SYMBOLS_LAZILY
    If enabled, JUCE will load libcurl lazily when required (for example, when WebInputStream
    is used). Enabling this flag may also help with library dependency errors as linking
    libcurl at compile-time may instruct the linker to hard depend on a specific version
    of libcurl. It's also useful if you want to limit the amount of JUCE dependencies and
    you are not using WebInputStream or the URL classes.
*/
#ifndef JUCE_LOAD_CURL_SYMBOLS_LAZILY
#define JUCE_LOAD_CURL_SYMBOLS_LAZILY 1
#endif

/** Config: JUCE_CATCH_UNHANDLED_EXCEPTIONS
    If enabled, this will add some exception-catching code to forward unhandled exceptions
    to your JUCEApplicationBase::unhandledException() callback.
*/
#ifndef JUCE_CATCH_UNHANDLED_EXCEPTIONS
#define JUCE_CATCH_UNHANDLED_EXCEPTIONS 0
#endif

/** Config: JUCE_ALLOW_STATIC_NULL_VARIABLES
    If disabled, this will turn off dangerous static globals like String::empty, var::null, etc
    which can cause nasty order-of-initialisation problems if they are referenced during static
    constructor code.
*/
#ifndef JUCE_ALLOW_STATIC_NULL_VARIABLES
#define JUCE_ALLOW_STATIC_NULL_VARIABLES 0
#endif

/** Config: JUCE_STRICT_REFCOUNTEDPOINTER
    If enabled, this will make the ReferenceCountedObjectPtr class stricter about allowing
    itself to be cast directly to a raw pointer. By default this is disabled, for compatibility
    with old code, but if possible, you should always enable it to improve code safety!
*/
#ifndef JUCE_STRICT_REFCOUNTEDPOINTER
#define JUCE_STRICT_REFCOUNTEDPOINTER 0
#endif

/** Config: JUCE_ENABLE_ALLOCATION_HOOKS
    If enabled, this will add global allocation functions with built-in assertions, which may
    help when debugging allocations in unit tests.
*/
#ifndef JUCE_ENABLE_ALLOCATION_HOOKS
#define JUCE_ENABLE_ALLOCATION_HOOKS 0
#endif

/** Config: JUCE_PROFILING_CATEGORIES
    If enabled, this will add global profiling categories to the profiler. The "yup" category should
    always be defined, only additional categories should be provided (note the first comma).
    Format of the categories is like:
    ```
        #define JUCE_PROFILING_CATEGORIES \
            , perfetto::Category ("custom1") \
            , perfetto::Category ("custom2")
    ```
*/
#ifndef JUCE_PROFILING_CATEGORIES
#define JUCE_PROFILING_CATEGORIES
#endif

/** Config: JUCE_PROFILING_FILE_PREFIX
    If provided, it will be used as prefix for profilation files generated. By default it will use "yup-profile".
*/
#ifndef JUCE_PROFILING_FILE_PREFIX
#define JUCE_PROFILING_FILE_PREFIX "yup-profile"
#endif

#ifndef JUCE_STRING_UTF_TYPE
#define JUCE_STRING_UTF_TYPE 8
#endif

//==============================================================================
//==============================================================================

#if JUCE_CORE_INCLUDE_NATIVE_HEADERS
#include "native/juce_BasicNativeHeaders.h"
#endif

#if JUCE_WINDOWS
#undef small
#endif

#include "system/juce_StandardHeader.h"

namespace juce
{
class StringRef;
class MemoryBlock;
class File;
class InputStream;
class OutputStream;
class DynamicObject;
class FileInputStream;
class FileOutputStream;
class XmlElement;

extern JUCE_API bool JUCE_CALLTYPE juce_isRunningUnderDebugger() noexcept;
extern JUCE_API void JUCE_CALLTYPE logAssertion (const char* file, int line) noexcept;
extern JUCE_API void JUCE_CALLTYPE logAssertion (const wchar_t* file, int line) noexcept;
} // namespace juce

#include "misc/juce_EnumHelpers.h"
#include "memory/juce_Memory.h"
#include "maths/juce_MathsFunctions.h"
#include "memory/juce_ByteOrder.h"
#include "memory/juce_Atomic.h"
#include "text/juce_CharacterFunctions.h"

JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4514 4996)

#include "text/juce_CharPointer_UTF8.h"
#include "text/juce_CharPointer_UTF16.h"
#include "text/juce_CharPointer_UTF32.h"
#include "text/juce_CharPointer_ASCII.h"

JUCE_END_IGNORE_WARNINGS_MSVC

#include "misc/juce_MetaProgramming.h"
#include "text/juce_String.h"
#include "text/juce_StringRef.h"
#include "logging/juce_Logger.h"
#include "misc/juce_Functional.h"
#include "containers/juce_Span.h"
#include "memory/juce_LeakedObjectDetector.h"
#include "memory/juce_ContainerDeletePolicy.h"
#include "memory/juce_HeapBlock.h"
#include "memory/juce_MemoryBlock.h"
#include "memory/juce_ReferenceCountedObject.h"
#include "memory/juce_ScopedPointer.h"
#include "memory/juce_OptionalScopedPointer.h"
#include "containers/juce_Optional.h"
#include "containers/juce_Enumerate.h"
#include "containers/juce_ScopedValueSetter.h"
#include "memory/juce_Singleton.h"
#include "memory/juce_WeakReference.h"
#include "threads/juce_ScopedLock.h"
#include "threads/juce_CriticalSection.h"
#include "maths/juce_Range.h"
#include "maths/juce_NormalisableRange.h"
#include "maths/juce_StatisticsAccumulator.h"
#include "containers/juce_ElementComparator.h"
#include "containers/juce_ArrayAllocationBase.h"
#include "containers/juce_ArrayBase.h"
#include "containers/juce_Array.h"
#include "containers/juce_LinkedListPointer.h"
#include "misc/juce_ScopeGuard.h"
#include "threads/juce_ThreadValueInitialiser.h"
#include "containers/juce_ListenerList.h"
#include "containers/juce_OwnedArray.h"
#include "containers/juce_ReferenceCountedArray.h"
#include "containers/juce_SortedSet.h"
#include "containers/juce_SparseSet.h"
#include "containers/juce_AbstractFifo.h"
#include "containers/juce_SingleThreadedAbstractFifo.h"
#include "text/juce_NewLine.h"
#include "text/juce_StringPool.h"
#include "text/juce_Identifier.h"
#include "text/juce_StringArray.h"
#include "text/juce_StringPairArray.h"
#include "system/juce_SystemStats.h"
#include "memory/juce_HeavyweightLeakedObjectDetector.h"
#include "text/juce_TextDiff.h"
#include "text/juce_LocalisedStrings.h"
#include "text/juce_Base64.h"
#include "misc/juce_FlagSet.h"
#include "misc/juce_Result.h"
#include "misc/juce_ResultValue.h"
#include "misc/juce_Uuid.h"
#include "misc/juce_ConsoleApplication.h"
#include "containers/juce_Variant.h"
#include "containers/juce_NamedValueSet.h"
#include "javascript/juce_JSON.h"
#include "containers/juce_DynamicObject.h"
#include "containers/juce_HashMap.h"
#include "containers/juce_FixedSizeFunction.h"
#include "time/juce_RelativeTime.h"
#include "time/juce_Time.h"
#include "time/juce_TimeoutDetector.h"
#include "streams/juce_InputStream.h"
#include "streams/juce_OutputStream.h"
#include "streams/juce_BufferedInputStream.h"
#include "streams/juce_MemoryInputStream.h"
#include "streams/juce_MemoryOutputStream.h"
#include "streams/juce_SubregionStream.h"
#include "streams/juce_InputSource.h"
#include "files/juce_File.h"
#include "files/juce_DirectoryIterator.h"
#include "files/juce_RangedDirectoryIterator.h"
#include "files/juce_FileInputStream.h"
#include "files/juce_FileOutputStream.h"
#include "files/juce_FileSearchPath.h"
#include "files/juce_MemoryMappedFile.h"
#include "files/juce_TemporaryFile.h"
#include "files/juce_FileFilter.h"
#include "files/juce_WildcardFileFilter.h"
#include "streams/juce_FileInputSource.h"
#include "logging/juce_FileLogger.h"
#include "javascript/juce_JSONUtils.h"
#include "serialisation/juce_Serialisation.h"
#include "javascript/juce_JSONSerialisation.h"
#include "javascript/juce_Javascript.h"
#include "maths/juce_BigInteger.h"
#include "maths/juce_Expression.h"
#include "maths/juce_Random.h"
#include "misc/juce_RuntimePermissions.h"
#include "misc/juce_WindowsRegistry.h"
#include "threads/juce_ChildProcess.h"
#include "threads/juce_DynamicLibrary.h"
#include "threads/juce_InterProcessLock.h"
#include "threads/juce_Process.h"
#include "threads/juce_SpinLock.h"
#include "threads/juce_WaitableEvent.h"
#include "threads/juce_Thread.h"
#include "threads/juce_HighResolutionTimer.h"
#include "threads/juce_ThreadLocalValue.h"
#include "threads/juce_ThreadPool.h"
#include "threads/juce_TimeSliceThread.h"
#include "threads/juce_ReadWriteLock.h"
#include "threads/juce_ScopedReadLock.h"
#include "threads/juce_ScopedWriteLock.h"
#include "network/juce_IPAddress.h"
#include "network/juce_MACAddress.h"
#include "network/juce_NamedPipe.h"
#include "network/juce_Socket.h"
#include "network/juce_URL.h"
#include "network/juce_WebInputStream.h"
#include "streams/juce_URLInputSource.h"
#include "time/juce_PerformanceCounter.h"
#include "unit_tests/juce_UnitTest.h"
#include "xml/juce_XmlDocument.h"
#include "xml/juce_XmlElement.h"
#include "zip/juce_GZIPCompressorOutputStream.h"
#include "zip/juce_GZIPDecompressorInputStream.h"
#include "zip/juce_ZipFile.h"
#include "containers/juce_PropertySet.h"
#include "memory/juce_SharedResourcePointer.h"
#include "memory/juce_AllocationHooks.h"
#include "memory/juce_Reservoir.h"
#include "files/juce_AndroidDocument.h"
#include "streams/juce_AndroidDocumentInputSource.h"

#include "detail/juce_CallbackListenerList.h"

#if JUCE_CORE_INCLUDE_OBJC_HELPERS && (JUCE_MAC || JUCE_IOS)
#include "native/juce_CFHelpers_apple.h"
#include "native/juce_ObjCHelpers_apple.h"
#endif

#if JUCE_CORE_INCLUDE_COM_SMART_PTR && JUCE_WINDOWS
#include "native/juce_ComSmartPtr_windows.h"
#endif

#if JUCE_CORE_INCLUDE_JNI_HELPERS && JUCE_ANDROID
#include <jni.h>
#include "native/juce_JNIHelpers_android.h"
#endif

#if JUCE_UNIT_TESTS
#include "unit_tests/juce_UnitTestCategories.h"
#endif

#if JUCE_ENABLE_PROFILING
JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4267)
#include <perfetto.h>
JUCE_END_IGNORE_WARNINGS_MSVC
#endif

#include "profiling/juce_Profiler.h"

#ifndef DOXYGEN
namespace juce
{
/*
    As the very long class names here try to explain, the purpose of this code is to cause
    a linker error if not all of your compile units are consistent in the options that they
    enable before including JUCE headers. The reason this is important is that if you have
    two cpp files, and one includes the juce headers with debug enabled, and another does so
    without that, then each will be generating code with different class layouts, and you'll
    get subtle and hard-to-track-down memory corruption!
 */
#if JUCE_DEBUG
struct JUCE_API this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_debug_mode
{
    this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_debug_mode() noexcept;
};

static this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_debug_mode compileUnitMismatchSentinel;
#else
struct JUCE_API this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_release_mode
{
    this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_release_mode() noexcept;
};

static this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_release_mode compileUnitMismatchSentinel;
#endif
} // namespace juce
#endif

JUCE_END_IGNORE_WARNINGS_MSVC

// In DLL builds, need to disable this warnings for other modules
#if defined(JUCE_DLL_BUILD) || defined(JUCE_DLL)
JUCE_IGNORE_MSVC (4251)
#endif

namespace yup
{
using namespace ::juce;
} // namespace yup
