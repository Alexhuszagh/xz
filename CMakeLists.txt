#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

cmake_minimum_required(VERSION 3.4)
project(Xz C)
set(C_STANDARD 99)

# VERSION
# -------

# TODO: this should really use a cross-platform version of the sed script
set(${PROJECT}_VERSION_MAJOR 5)
set(${PROJECT}_VERSION_MINOR 3)
set(${PROJECT}_VERSION_PATCH 0)

set(PACKAGE_NAME "XZ Utils")
set(PACKAGE_VERSION "5.3.0alpha")
add_definitions(
    -DPACKAGE_NAME="${PACKAGE_NAME}"
    -DPACKAGE_STRING="XZ Utils 5.3.0alpha"
    -DPACKAGE_TARNAME="xz"
    -DPACKAGE_VERSION="${PACKAGE_VERSION}"
    -DPACKAGE_URL="http://tukaani.org/xz/"
    -DPACKAGE_BUGREPORT="lasse.collin@tukaani.org"
)

# ARGUMENTS
# ---------

option(BUILD_STATIC "Build static liblzma" ON)
option(BUILD_DOCUMENTATION "Build LZMA documentation" ON)
if(UNIX)
    option(BUILD_XZ "Build XZ compressor/decompressor" OFF)
    option(BUILD_LZMA_INFO "Build LZMA information" OFF)
    option(BUILD_SCRIPTS "Build LZMA scripts" OFF)
    option(BUILD_TESTS "Build LZMA tests" OFF)
endif()

# OPTIONS
option(ENABLE_ASSSEMBLER "Enable optimized assembly instructions" ON)
option(ENABLE_ASSUME_RAM "Enable RAM assumptions on systems without RAM detection" ON)
option(ENABLE_SCRIPTS "Build LZMA xzdiff, xzgrep, xzmore and xzless" ON)
option(ENABLE_SMALL "Build small version of LZMA" OFF)
option(ENABLE_SYMBOL_VERSIONS "Build LZMA with version symbols" ON)
option(ENABLE_THREADS "Build LZMA with threading support" ON)

if(ENABLE_ASSSEMBLER)
    enable_language(ASM)
endif()

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

if(ENABLE_ENCODERS)
    add_definitions(-DHAVE_ENCODERS -DHAVE_DECODERS)
endif()

set(ENABLE_LZ ${ENABLE_LZMA1})
set(ENABLE_SIMPLE ${ENABLE_DELTA} OR ${ENABLE_X86} OR ${ENABLE_POWERPC} OR ${ENABLE_IA64} OR ${ENABLE_ARM} OR ${ENABLE_ARMTHUMB} OR ${ENABLE_SPARC})

if(ENABLE_LZMA2 AND NOT ENABLE_LZMA1)
    message(FATAL_ERROR "Lzma2 requires that Lzma1 is also enabled.")
endif()

# MATCH FINDERS
if(ENABLE_LZMA1)
    option(ENABLE_HC2 "Enable HC2 (hash chain compression)" ON)
    option(ENABLE_HC3 "Enable HC3 (hash chain compression)" ON)
    option(ENABLE_HC4 "Enable HC4 (hash chain compression)" ON)
    option(ENABLE_BT2 "Enable BT2 (binary tree compression)" ON)
    option(ENABLE_BT3 "Enable BT3 (binary tree compression)" ON)
    option(ENABLE_BT4 "Enable BT4 (binary tree compression)" ON)
    set(ENABLE_MATCH_FINDERS ${ENABLE_HC2} OR ${ENABLE_HC3} OR ${ENABLE_HC4} OR ${ENABLE_BT2} OR ${ENABLE_BT3} OR ${ENABLE_BT4})
endif()

if(ENABLE_LZMA1 AND NOT ENABLE_MATCH_FINDERS)
    message(FATAL_ERROR "You must enable at least one match filter with the Lzma filters.")
endif()

# HEADERS
# -------

include(CheckIncludeFile)

add_definitions(-DSTDC_HEADERS=1)

# Check if header can be found. If it can, set uppercase variable
# of version and define includes for the header.
#
# Args:
#   header          Path to header
#
# Example:
#   CheckIncludeFile(sys/stat.h)
#
macro(DefineIncludeHeader header)
    string(REGEX REPLACE "[.\\/]" "_" headerName ${header})
    string(TOUPPER ${headerName} headerName)
    CHECK_INCLUDE_FILE(${header} ${headerName})
    if(${headerName})
        add_definitions(-DHAVE_${headerName}=1)
    endif()
endmacro(DefineIncludeHeader)

# Call DefineIncludeHeader for each argument.
#
# Example:
#   DefineIncludeHeaders(string.h stdio.h sys/stat.h)
#
macro(DefineIncludeHeaders)
    set(headers ${ARGN})
    foreach(header ${headers})
        DefineIncludeHeader(${header})
    endforeach(header)
endmacro(DefineIncludeHeaders)

DefineIncludeHeaders(
    stdbool.h
    sys/types.h
    sys/stat.h
    stdlib.h
    string.h
    memory.h
    strings.h
    inttypes.h
    stdint.h
    unistd.h
    fcntl.h
    limits.h
    sys/time.h
    immintrin.h
    CommonCrypto/CommonDigest.h
    sha256.h
    sha2.h
)

# FUNCTIONS
# ---------

include(CheckFunctionExists)

# Check if function exists, and add definition if it does.
#
# Args:
#   function        Function name
#
# Example:
#   CheckIncludeFile(clock_gettime)
#
macro(DefineFunctionExists function)
    string(TOUPPER ${function} functionName)
    CHECK_FUNCTION_EXISTS(${function} ${functionName})
    if(${functionName})
        add_definitions(-DHAVE_${functionName}=1)
    endif()
endmacro(DefineFunctionExists)


# Call DefineFunctionExists for each argument.
#
# Example:
#   DefineFunctionsExist(strdup strndup strerror)
#
macro(DefineFunctionsExist)
    set(functions ${ARGN})
    foreach(function ${functions})
        DefineFunctionExists(${function})
    endforeach(function)
endmacro(DefineFunctionsExist)


DefineFunctionsExist(
    clock_gettime
    pthread_condattr_setclock
    futimens
    futimes
    futimesat
    utimes
    _futime
    utime
    posix_fadvise
    CC_SHA256_Init
    SHA256Init
    SHA256_Init
    _mm_movemask_epi8
)

CHECK_FUNCTION_EXISTS(CLOCK_MONOTONIC DECL_CLOCK_MONOTONIC)
if(DECL_CLOCK_MONOTONIC)
    add_definitions(-DHAVE_DECL_CLOCK_MONOTONIC=1)
endif()

# LITERALS
# --------

include(CheckTypeSize)

# Unlike Autotools, Cmake cannot define types, so all must be required.
CHECK_TYPE_SIZE(uint8_t UINT8_T)
CHECK_TYPE_SIZE(uint16_t UINT16_T)
CHECK_TYPE_SIZE(uint32_t UINT32_T)
CHECK_TYPE_SIZE(uint64_t UINT64_T)
CHECK_TYPE_SIZE(uintptr_t UINTPTR_T)
if(NOT UINT8_T OR NOT UINT16_T OR NOT UINT32_T OR NOT UINT64_T OR NOT UINTPTR_T)
    message(FATAL_ERROR "Cannot find required literals, use the Autotools configure")
endif()

CHECK_TYPE_SIZE(SHA2_CTX SHA2_CTX)
if(SHA2_CTX)
    add_definitions(-DHAVE_SHA2_CTX=1)
endif()

CHECK_TYPE_SIZE(size_t SIZEOF_SIZE_T)
add_definitions(-DSIZEOF_SIZE_T=${SIZEOF_SIZE_T})

if(NOT STDBOOL_H)
    CHECK_TYPE_SIZE(_Bool HAVE_BOOL)
    if(HAVE_BOOL)
        add_definitions(-DHAVE__BOOL=1)
    endif()
endif()

# STRUCTS
# -------

include(CheckStructHasMember)

# Check if struct has member, and add preprocessor definition if it does.
#
# Args:
#   struct              Struct name
#   member              Member name (can include members of members)
#   header              Header struct is defined in
#
# Example:
#   DefineStructHasMember(stat st_atim.tv_nsec sys/stat.h)
#
macro(DefineStructHasMember struct member header)
    # make suitable struct member name
    string(REGEX REPLACE "[.\\/]" "_" structMemberName ${member})
    set(structMemberName struct_${struct}_${structMemberName})
    string(TOUPPER ${structMemberName} structMemberName)

    CHECK_STRUCT_HAS_MEMBER("struct ${struct}" ${member} ${header} ${structMemberName}
        LANGUAGE C
    )
    if(${structMemberName})
        add_definitions(-DHAVE_${structMemberName}=1)
    endif()
endmacro(DefineStructHasMember)


if(SYS_STAT_H)
    DefineStructHasMember(stat st_atim.tv_nsec sys/stat.h)
    DefineStructHasMember(stat st_atimespec.tv_nsec sys/stat.h)
    DefineStructHasMember(stat st_atimensec sys/stat.h)
    DefineStructHasMember(stat st_uatime sys/stat.h)
    DefineStructHasMember(stat st_atim.st__tim.tv_nsec sys/stat.h)
endif()

# THREADS
# -------

include(FindThreads)

if(ENABLE_THREADS)
    if(CMAKE_USE_PTHREADS_INIT AND NOT WIN32)
        # mingw-w64 is missing sigfillset
        add_definitions(-DMYTHREAD_POSIX=1)
    elseif(CMAKE_USE_WIN32_THREADS_INIT)
        # CMake doesn't support Win95 threads, Win32 threads are Vista
        add_definitions(-DMYTHREAD_VISTA=1)
    else()
        message(WARNING "Threads requested but no suitable thread implementation found")
        set(ENABLE_THREADS OFF)
    endif()
endif()

# SYSTEM
# ------

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
include(Architecture)
include(TestBigEndian)

TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    add_definitions(-DWORDS_BIGENDIAN=1)
endif()

if(ARCH EQUAL 32)
elseif(ARCH EQUAL 64)
    add_definitions(-D_FILE_OFFSET_BITS=64)
    add_definitions(-D_LARGE_FILES=1)
else()
    message(FATAL_ERROR "XzUtils assumes 32 or 64 bit architecture")
endif()

if(UNIX)
    #add_definitions(-DHAVE_UNISTD_H=1)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    add_definitions(-D_DARWIN_USE_64_BIT_INODE=1)
endif()

if(MINIX)
    # Minix users need to provide their own CMake toolchain that defines MINIX
endif()

# OPTION DEFINITIONS
# ------------------

if(ENABLE_ASSUME_RAM)
    add_definitions(-DASSUME_RAM=128)
endif()

if(ENABLE_HC2)
    add_definitions(-DHAVE_MF_HC2=1)
endif()

if(ENABLE_HC3)
    add_definitions(-DHAVE_MF_HC3=1)
endif()

if(ENABLE_HC4)
    add_definitions(-DHAVE_MF_HC4=1)
endif()

if(ENABLE_BT2)
    add_definitions(-DHAVE_MF_BT2=1)
endif()

if(ENABLE_BT3)
    add_definitions(-DHAVE_MF_BT3=1)
endif()

if(ENABLE_BT4)
    add_definitions(-DHAVE_MF_BT4=1)
endif()

if(ENABLE_CRC32)
    add_definitions(-DHAVE_CHECK_CRC32=1)
endif()

if(ENABLE_CRC64)
    add_definitions(-DHAVE_CHECK_CRC64=1)
endif()

if(ENABLE_LZMA1)
    add_definitions(-DHAVE_ENCODER_LZMA1=1)
    add_definitions(-DHAVE_DECODER_LZMA1=1)
endif()

if(ENABLE_LZMA2)
    add_definitions(-DHAVE_ENCODER_LZMA2=1)
    add_definitions(-DHAVE_DECODER_LZMA2=1)
endif()

if(ENABLE_X86)
    add_definitions(-DHAVE_ENCODER_X86=1)
    add_definitions(-DHAVE_DECODER_X86=1)
endif()

if(ENABLE_POWERPC)
    add_definitions(-DHAVE_ENCODER_POWERPC=1)
    add_definitions(-DHAVE_DECODER_POWERPC=1)
endif()

if(ENABLE_IA64)
    add_definitions(-DHAVE_ENCODER_IA64=1)
    add_definitions(-DHAVE_DECODER_IA64=1)
endif()

if(ENABLE_ARM)
    add_definitions(-DHAVE_ENCODER_ARM=1)
    add_definitions(-DHAVE_DECODER_ARM=1)
endif()

if(ENABLE_ARMTHUMB)
    add_definitions(-DHAVE_ENCODER_ARMTHUMB=1)
    add_definitions(-DHAVE_DECODER_ARMTHUMB=1)
endif()

if(ENABLE_SPARC)
    add_definitions(-DHAVE_ENCODER_SPARC=1)
    add_definitions(-DHAVE_DECODER_SPARC=1)
endif()

if(ENABLE_DELTA)
    add_definitions(-DHAVE_ENCODER_DELTA=1)
    add_definitions(-DHAVE_DECODER_DELTA=1)
endif()

if(MINGW AND BUILD_STATIC)
    # STATIC RUNTIME
    set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS}")
endif()

# LIBRARIES
# ---------

set(XZ_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(XZ_LIB_DIR "${XZ_PROJECT_DIR}/lib")

add_subdirectory(src)

# TESTS
# -----

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# DOCUMENTATION
# -------------

if(BUILD_DOCUMENTATION)
    find_program(DOXYGEN doxygen)
    if(NOT DOXYGEN)
        message(WARNING "Doxygen must be installed to build documentation.")
    else()
        # read doxyfile
        set(DOXYFILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
        file(READ "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" DOXYFILE)

        # replace autotools strings
        string(REPLACE "@PACKAGE_NAME@" "${PACKAGE_NAME}" DOXYFILE ${DOXYFILE})
        string(REPLACE "@PACKAGE_VERSION@" "${PACKAGE_VERSION}" DOXYFILE ${DOXYFILE})
        string(REPLACE "@top_srcdir@" "${CMAKE_CURRENT_SOURCE_DIR}" DOXYFILE ${DOXYFILE})

        # write doxyfile
        file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile" ${DOXYFILE})

        # add target
        add_custom_target(doc
            COMMAND ${DOXYGEN} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
        )
    endif()
endif()
