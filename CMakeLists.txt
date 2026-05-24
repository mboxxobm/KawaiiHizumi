cmake_minimum_required(VERSION 3.22)

if(APPLE AND NOT DEFINED CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "macOS architectures" FORCE)
endif()

if(APPLE AND NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version" FORCE)
endif()

project(KawaiiHizumi VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(KAWAII_COPY_PLUGIN_AFTER_BUILD "Copy built plugins to the local plugin folders" ON)

set(KAWAII_HIZUMI_FORMATS VST3 Standalone)

if(APPLE)
    list(APPEND KAWAII_HIZUMI_FORMATS AU)
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    message(FATAL_ERROR "JUCE was not found. Run: git clone --depth 1 https://github.com/juce-framework/JUCE.git JUCE")
endif()

add_subdirectory(JUCE)

juce_add_plugin(KawaiiHizumi
    COMPANY_NAME "Codex Tools"
    BUNDLE_ID "com.codextools.kawaiihizumi"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD ${KAWAII_COPY_PLUGIN_AFTER_BUILD}
    PLUGIN_MANUFACTURER_CODE CDEX
    PLUGIN_CODE KWDV
    FORMATS ${KAWAII_HIZUMI_FORMATS}
    PRODUCT_NAME "Kawaii Hizumi")

juce_generate_juce_header(KawaiiHizumi)

target_sources(KawaiiHizumi
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp)

target_compile_definitions(KawaiiHizumi
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(KawaiiHizumi
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
