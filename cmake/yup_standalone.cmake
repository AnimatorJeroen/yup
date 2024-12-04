# ==============================================================================
#
#   This file is part of the YUP library.
#   Copyright (c) 2024 - kunitoki@gmail.com
#
#   YUP is an open source library subject to open-source licensing.
#
#   The code included in this file is provided under the terms of the ISC license
#   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
#   To use, copy, modify, and/or distribute this software for any purpose with or
#   without fee is hereby granted provided that the above copyright notice and
#   this permission notice appear in all copies.
#
#   YUP IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
#   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
#   DISCLAIMED.
#
# ==============================================================================

#==============================================================================

function (yup_standalone_app)
    # ==== Fetch options
    set (options CONSOLE)
    set (one_value_args
        TARGET_NAME TARGET_VERSION TARGET_IDE_GROUP TARGET_APP_ID TARGET_APP_NAMESPACE
        CUSTOM_PLIST CUSTOM_SHELL)
    set (multi_value_args
        DEFINITIONS MODULES PRELOAD_FILES LINK_OPTIONS)

    cmake_parse_arguments (YUP_ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    set (target_name "${YUP_ARG_TARGET_NAME}")
    set (target_version "${YUP_ARG_TARGET_VERSION}")
    set (target_app_id "${YUP_ARG_TARGET_APP_ID}")
    set (target_app_namespace "${YUP_ARG_TARGET_APP_NAMESPACE}")
    set (additional_definitions "")
    set (additional_options "")
    set (additional_libraries "")
    set (additional_link_options "")

    _yup_make_short_version ("${target_version}" target_version_short)

    # ==== Setup Android platform, build gradle stage
    if (YUP_TARGET_ANDROID)
        _yup_prepare_gradle_android(
            TARGET_NAME ${target_name}
            APPLICATION_ID ${target_app_id}
            APPLICATION_NAMESPACE ${target_app_namespace}
            APPLICATION_VERSION ${target_version})
        return()
    endif()

    # ==== Find dependencies
    if (NOT "${yup_platform}" MATCHES "^(emscripten|ios)$")
        _yup_fetch_glfw3()
        list (APPEND additional_libraries glfw)
    endif()

    # ==== Enable profiling
    if (YUP_ENABLE_PROFILING AND NOT "${target_name}" STREQUAL "yup_tests")
        list (APPEND additional_definitions JUCE_ENABLE_PROFILING=1 YUP_ENABLE_PROFILING=1)
        list (APPEND additional_libraries perfetto::perfetto)
    endif()

    # ==== Prepare executable
    set (executable_options "")
    if (NOT YUP_ARG_CONSOLE AND "${yup_platform}" MATCHES "^(win32)$")
        set (executable_options "WIN32")
    endif()

    if ("${yup_platform}" MATCHES "^(android)$")
        add_library (${target_name} SHARED)
    else()
        add_executable (${target_name} ${executable_options})
    endif()

    target_compile_features (${target_name} PRIVATE cxx_std_17)

    # ==== Per platform configuration
    if ("${yup_platform}" MATCHES "^(osx|ios)$")
        if (NOT YUP_ARG_CONSOLE)
            if (NOT DEFINED YUP_ARG_CUSTOM_PLIST)
                set (YUP_ARG_CUSTOM_PLIST "${CMAKE_SOURCE_DIR}/cmake/platforms/${yup_platform}/Info.plist")
            endif()

            set_target_properties (${target_name} PROPERTIES
                BUNDLE                                         ON
                CXX_EXTENSIONS                                 OFF
                MACOSX_BUNDLE_EXECUTABLE_NAME                  "${target_name}"
                MACOSX_BUNDLE_GUI_IDENTIFIER                   "org.kunitoki.yup.${target_name}"
                MACOSX_BUNDLE_BUNDLE_NAME                      "${target_name}"
                MACOSX_BUNDLE_BUNDLE_VERSION                   "${target_version}"
                MACOSX_BUNDLE_LONG_VERSION_STRING              "${target_version}"
                MACOSX_BUNDLE_SHORT_VERSION_STRING             "${target_version_short}"
                MACOSX_BUNDLE_ICON_FILE                        "Icon.icns"
                MACOSX_BUNDLE_INFO_PLIST                       "${YUP_ARG_CUSTOM_PLIST}"
                #RESOURCE                                       "${RESOURCE_FILES}"
                #XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY             ""
                XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED          OFF
                XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT       dwarf
                XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN ON
                XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME        OFF)
        endif()

        set_target_properties (${target_name} PROPERTIES
            XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC OFF)

    elseif ("${yup_platform}" MATCHES "^(emscripten)$")
        if (NOT YUP_ARG_CONSOLE)
            set_target_properties (${target_name} PROPERTIES SUFFIX ".html")

            list (APPEND additional_definitions RIVE_WEBGL=1)
            list (APPEND additional_link_options -sUSE_GLFW=3 -sMAX_WEBGL_VERSION=2)
        endif()

        if (NOT DEFINED YUP_ARG_CUSTOM_SHELL)
            set (YUP_ARG_CUSTOM_SHELL "${CMAKE_SOURCE_DIR}/cmake/platforms/${yup_platform}/shell.html")
        endif()

        list (APPEND additional_options
            $<$<CONFIG:DEBUG>:-O0 -g>
            $<$<CONFIG:RELEASE>:-O3>
            -fexceptions
            -pthread
            -sDISABLE_EXCEPTION_CATCHING=0)

        list (APPEND additional_link_options
            $<$<CONFIG:DEBUG>:-gsource-map -g>
            -fexceptions
            -pthread
            -sWASM=1
            -sWASM_WORKERS=1
            -sAUDIO_WORKLET=1
            -sSHARED_MEMORY=1
            -sALLOW_MEMORY_GROWTH=0
            -sASSERTIONS=1
            -sDISABLE_EXCEPTION_CATCHING=0
            -sERROR_ON_UNDEFINED_SYMBOLS=1
            -sDEMANGLE_SUPPORT=1
            -sSTACK_OVERFLOW_CHECK=2
            -sPTHREAD_POOL_SIZE=8
            -sFORCE_FILESYSTEM=1
            -sNODERAWFS=0
            -sFETCH=1
            -sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE='$dynCall'
            --shell-file "${YUP_ARG_CUSTOM_SHELL}")

        foreach (preload_file ${YUP_ARG_PRELOAD_FILES})
            list (APPEND additional_link_options --preload-file ${preload_file})
        endforeach()

        set (target_copy_dest "$<TARGET_FILE_DIR:${target_name}>")
        add_custom_command(
            TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_SOURCE_DIR}/cmake/platforms/${yup_platform}/mini-coi.js"
                "${target_copy_dest}/mini-coi.js")

    endif()

    if (YUP_ARG_TARGET_IDE_GROUP)
        set_target_properties (${target_name} PROPERTIES FOLDER "${YUP_ARG_TARGET_IDE_GROUP}")
    endif()

    # ==== Definitions and link libraries
    target_compile_options (${target_name} PRIVATE
        ${additional_options}
        ${YUP_ARG_OPTIONS})

    target_compile_definitions (${target_name} PRIVATE
        $<IF:$<CONFIG:Debug>,DEBUG=1,NDEBUG=1>
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
        JUCE_STANDALONE_APPLICATION=1
        ${additional_definitions}
        ${YUP_ARG_DEFINITIONS})

    target_link_options (${target_name} PRIVATE
        ${additional_link_options}
        ${YUP_ARG_LINK_OPTIONS})

    target_link_libraries (${target_name} PRIVATE
        ${additional_libraries}
        ${YUP_ARG_MODULES})

endfunction()
