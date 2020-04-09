
cmake_minimum_required (VERSION 3.4.3)
project(wormhole)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(SDK_ROOT "${ROOT}/lib/vst3sdk")
list(APPEND CMAKE_MODULE_PATH "${ROOT}/lib/vst3sdk/cmake/modules")
include(SMTG_VST3_SDK)

setupPlatformToolset()

#-----------------------------#
# VST SDK build
#-----------------------------#
set(public_sdk_SOURCE_DIR ${SDK_ROOT}/public.sdk)
set(pluginterfaces_SOURCE_DIR ${SDK_ROOT}/pluginterfaces)
# add_subdirectory("${SDK_ROOT}/build/lib/Debug")

# CORE AUDIO SDK, AAX SDK
setupCoreAudioSupport()
setupAaxSupport()

set(VST_SDK TRUE)
add_subdirectory(${SDK_ROOT}/pluginterfaces)
add_subdirectory(${SDK_ROOT}/base)
add_subdirectory(${SDK_ROOT}/public.sdk)
add_subdirectory(${SDK_ROOT}/public.sdk/source/vst/interappaudio)


set(SDK_IDE_HOSTING_EXAMPLES_FOLDER FOLDER "HostingExamples")
add_subdirectory(${SDK_ROOT}/public.sdk/samples/vst-hosting)


# TODO: Add Wrappers


# uWebSocket build
set (WS_ROOT "${ROOT}/lib/uWebSockets")
# set (WS_FLAGS "-lpthread -std=c++17 -Wc++17-extensions")
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WS_FLAGS}")
# set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "${WS_ROOT}/uSockets/*.o -lz")
add_subdirectory(${WS_ROOT})

# add_subdirectory(${WS_ROOT}/src)
# include_directories(${WS_ROOT}/src ${WS_ROOT}/uSockets/src)


set(SMTG_MYPLUGINS_SRC_PATH "${ROOT}/src")
# set(SDK_IDE_MYPLUGINS_FOLDER FOLDER "src")
if(EXISTS ${SMTG_MYPLUGINS_SRC_PATH})
  set(SMTG_MYPLUGINS_BIN_PATH "${ROOT}/build/bin")
    add_subdirectory(${SMTG_MYPLUGINS_SRC_PATH} ${SMTG_MYPLUGINS_BIN_PATH})
endif()


# KFR build
