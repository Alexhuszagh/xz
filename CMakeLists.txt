#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

cmake_minimum_required(VERSION 3.4)
project(Xz C)

# VERSION
# -------

set(${PROJECT}_VERSION_MAJOR 5)
set(${PROJECT}_VERSION_MINOR 3)
set(${PROJECT}_VERSION_PATCH 0)

# ARGUMENTS
# ---------

option(BUILD_STATIC "Build static liblzma" ON)
option(BUILD_XZ "Build XZ compressor/decompressor" OFF)
option(BUILD_LZMA_INFO "Build LZMA information" OFF)
option(BUILD_SCRIPTS "Build LZMA scripts" OFF)

# OPTIONS
option(ENABLE_ASSSEMBLER "Enable optimized assembly instructions" ON)
option(ENABLE_DOC "Build LZMA documentation" ON)
option(ENABLE_SCRIPTS "Build LZMA xzdiff, xzgrep, xzmore and xzless" ON)
option(ENABLE_SMALL "Build small version of LZMA" OFF)
option(ENABLE_THREADS "Build LZMA with threading support" ON)

# CHECKS
set(ENABLE_CRC32 ON)
option(ENABLE_CRC64 "Enable CRC64 check" ON)
option(ENABLE_SHA256 "Enable Sha256 check" ON)
option(EXTERNAL_SHA256 "Use external Sha256" OFF)

# FILTERS
option(ENABLE_LZMA1 "Enable lzma1 filter" ON)
option(ENABLE_LZMA2 "Enable lzma2 filter" ON)
option(ENABLE_DELTA "Enable delta filter" ON)
option(ENABLE_X86 "Enable x86 filter" ON)
option(ENABLE_POWERPC "Enable PowerPC filter" ON)
option(ENABLE_IA64 "Enable IA64 filter" ON)
option(ENABLE_ARM "Enable arm filter" ON)
option(ENABLE_ARMTHUMB "Enable armthumb filter" ON)
option(ENABLE_SPARC "Enable sparc filter" ON)

set(ENABLE_ENCODERS ${ENABLE_LZMA1} OR ${ENABLE_LZMA2} OR ${ENABLE_DELTA} OR ${ENABLE_X86} OR ${ENABLE_POWERPC} OR ${ENABLE_IA64} OR ${ENABLE_ARM} OR ${ENABLE_ARMTHUMB} OR ${ENABLE_SPARC})
set(ENABLE_DECODERS ${ENABLE_ENCODERS})

# LIBRARIES
# ---------

set(XZ_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(XZ_LIB_DIR "${XZ_PROJECT_DIR}/lib")

add_subdirectory(src)
#
#set(XZ_SOURCES
#)
#
#set(XZDEC_SOURCES
#)
#
#set(LZMADEC_SOURCES
#)
#
#include_directories(
#    src/liblzma
#)
