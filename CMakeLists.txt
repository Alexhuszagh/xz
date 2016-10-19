#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

cmake_minimum_required(VERSION 3.4)
project(Xz C)
set(C_STANDARD 99)

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
option(ENABLE_ASSUME_RAM "Enable RAM assumptions on systems without RAM detection" ON)
option(ENABLE_DOC "Build LZMA documentation" ON)
option(ENABLE_SCRIPTS "Build LZMA xzdiff, xzgrep, xzmore and xzless" ON)
option(ENABLE_SMALL "Build small version of LZMA" OFF)
option(ENABLE_SYMBOL_VERSIONS "Build LZMA with version symbols" ON)
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

CHECK_INCLUDE_FILE(stdbool.h STDBOOL_H)
if(STDBOOL_H)
    add_definitions(-DHAVE_STDBOOL_H=1)
endif()

CHECK_INCLUDE_FILE(sys/types.h SYS_TYPES_H)
if(SYS_TYPES_H)
    add_definitions(-DHAVE_SYS_TYPES_H=1)
endif()

CHECK_INCLUDE_FILE(sys/stat.h SYS_STAT_H)
if(SYS_STAT_H)
    add_definitions(-DHAVE_SYS_STAT_H=1)
endif()

CHECK_INCLUDE_FILE(stdlib.h STDLIB_H)
if(STDLIB_H)
    add_definitions(-DHAVE_STDLIB_H=1)
endif()

CHECK_INCLUDE_FILE(string.h STRING_H)
if(STRING_H)
    add_definitions(-DHAVE_STRING_H=1)
endif()

CHECK_INCLUDE_FILE(memory.h MEMORY_H)
if(MEMORY_H)
    add_definitions(-DHAVE_MEMORY_H=1)
endif()

CHECK_INCLUDE_FILE(strings.h STRINGS_H)
if(STRINGS_H)
    add_definitions(-DHAVE_STRINGS_H=1)
endif()

CHECK_INCLUDE_FILE(inttypes.h INTTYPES_H)
if(INTTYPES_H)
    add_definitions(-DHAVE_INTTYPES_H=1)
endif()

CHECK_INCLUDE_FILE(stdint.h STDINT_H)
if(STDINT_H)
    add_definitions(-DHAVE_STDINT_H=1)
endif()

CHECK_INCLUDE_FILE(unistd.h UNISTD_H)
if(UNISTD_H)
    add_definitions(-DHAVE_UNISTD_H=1)
endif()

CHECK_INCLUDE_FILE(fcntl.h FCNTL_H)
if(FCNTL_H)
    add_definitions(-DHAVE_FCNTL_H=1)
endif()

CHECK_INCLUDE_FILE(limits.h LIMITS_H)
if(LIMITS_H)
    add_definitions(-DHAVE_LIMITS_H=1)
endif()

CHECK_INCLUDE_FILE(sys/time.h SYS_TIME_H)
if(SYS_TIME_H)
    add_definitions(-DHAVE_SYS_TIME_H=1)
endif()

CHECK_INCLUDE_FILE(immintrin.h IMMINTRIN_H)
if(IMMINTRIN_H)
    add_definitions(-DHAVE_IMMINTRIN_H=1)
endif()

CHECK_INCLUDE_FILE(immintrin.h IMMINTRIN_H)
if(IMMINTRIN_H)
    add_definitions(-DHAVE_IMMINTRIN_H=1)
endif()

CHECK_INCLUDE_FILE(CommonCrypto/CommonDigest.h COMMONCRYPTO_COMMONDIGEST_H)
if(COMMONCRYPTO_COMMONDIGEST_H)
    add_definitions(-DHAVE_COMMONCRYPTO_COMMONDIGEST_H=1)
endif()

CHECK_INCLUDE_FILE(sha256.h SHA256_H)
if(SHA256_H)
    add_definitions(-DHAVE_SHA256_H=1)
endif()

CHECK_INCLUDE_FILE(sha2.h SHA2_H)
if(SHA2_H)
    add_definitions(-DHAVE_SHA2_H=1)
endif()

# FUNCTIONS
# ---------

include(CheckFunctionExists)

CHECK_FUNCTION_EXISTS(clock_gettime CLOCK_GETTIME)
if(CLOCK_GETTIME)
    add_definitions(-DHAVE_CLOCK_GETTIME=1)
endif()

CHECK_FUNCTION_EXISTS(pthread_condattr_setclock PTHREAD_CONDATTR_SETCLOCK)
if(PTHREAD_CONDATTR_SETCLOCK)
    add_definitions(-DHAVE_PTHREAD_CONDATTR_SETCLOCK=1)
endif()

CHECK_FUNCTION_EXISTS(CLOCK_MONOTONIC DECL_CLOCK_MONOTONIC)
if(DECL_CLOCK_MONOTONIC)
    add_definitions(-DHAVE_DECL_CLOCK_MONOTONIC=1)
endif()

CHECK_FUNCTION_EXISTS(futimens FUTIMENS)
if(FUTIMENS)
    add_definitions(-DHAVE_FUTIMENS=1)
endif()

CHECK_FUNCTION_EXISTS(futimes FUTIMES)
if(FUTIMES)
    add_definitions(-DHAVE_FUTIMES=1)
endif()

CHECK_FUNCTION_EXISTS(futimesat FUTIMESAT)
if(FUTIMESAT)
    add_definitions(-DHAVE_FUTIMESAT=1)
endif()

CHECK_FUNCTION_EXISTS(utimes UTIMES)
if(UTIMES)
    add_definitions(-DHAVE_UTIMES=1)
endif()

CHECK_FUNCTION_EXISTS(_futime FUTIME)
if(FUTIME)
    add_definitions(-DHAVE__FUTIME=1)
endif()

CHECK_FUNCTION_EXISTS(utime UTIME)
if(UTIME)
    add_definitions(-DHAVE_UTIME=1)
endif()

CHECK_FUNCTION_EXISTS(posix_fadvise POSIX_FADVISE)
if(POSIX_FADVISE)
    add_definitions(-DHAVE_POSIX_FADVISE=1)
endif()

CHECK_FUNCTION_EXISTS(CC_SHA256_Init CC_SHA256_INIT)
if(CC_SHA256_INIT)
    add_definitions(-DHAVE_CC_SHA256_INIT=1)
endif()

CHECK_FUNCTION_EXISTS(SHA256Init SHA256INIT)
if(SHA256INIT)
    add_definitions(-DHAVE_SHA256INIT=1)
endif()

CHECK_FUNCTION_EXISTS(SHA256_Init SHA256_INIT)
if(SHA256_INIT)
    add_definitions(-DHAVE_SHA256_INIT=1)
endif()

CHECK_FUNCTION_EXISTS(_mm_movemask_epi8 MM_MOVEMASK_EPI8)
if(MM_MOVEMASK_EPI8)
    add_definitions(-DHAVE__MM_MOVEMASK_EPI8=1)
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

if(SYS_STAT_H)
    CHECK_STRUCT_HAS_MEMBER("struct stat" st_atim.tv_nsec sys/stat.h STRUCT_STAT_ST_ATIM_TV_NSEC
        LANGUAGE C
    )
    if (STRUCT_STAT_ST_ATIM_TV_NSEC)
        add_definitions(-DHAVE_STRUCT_STAT_ST_ATIM_TV_NSEC=1)
    endif()

    CHECK_STRUCT_HAS_MEMBER("struct stat" st_atimespec.tv_nsec sys/stat.h STRUCT_STAT_ST_ATIMESPEC_TV_NSEC
        LANGUAGE C
    )
    if (STRUCT_STAT_ST_ATIMESPEC_TV_NSEC)
        add_definitions(-DHAVE_STRUCT_STAT_ST_ATIMESPEC_TV_NSEC=1)
    endif()

    CHECK_STRUCT_HAS_MEMBER("struct stat" st_atimensec sys/stat.h STRUCT_STAT_ST_ATIMENSEC
        LANGUAGE C
    )
    if (STRUCT_STAT_ST_ATIMENSEC)
        add_definitions(-DHAVE_STRUCT_STAT_ST_ATIMENSEC=1)
    endif()

    CHECK_STRUCT_HAS_MEMBER("struct stat" st_uatime sys/stat.h STRUCT_STAT_ST_UATIME
        LANGUAGE C
    )
    if (STRUCT_STAT_ST_UATIME)
        add_definitions(-DHAVE_STRUCT_STAT_ST_UATIME=1)
    endif()

    CHECK_STRUCT_HAS_MEMBER("struct stat" st_atim.st__tim.tv_nsec sys/stat.h STRUCT_STAT_ST_ATIM_ST__TIM_TV_NSEC
        LANGUAGE C
    )
    if (STRUCT_STAT_ST_ATIM_ST__TIM_TV_NSEC)
        add_definitions(-DHAVE_STRUCT_STAT_ST_ATIM_ST__TIM_TV_NSEC=1)
    endif()
endif()

# THREADS
# -------

include(FindThreads)

if(ENABLE_THREADS)
    if(CMAKE_USE_PTHREADS_INIT)
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

# LIBRARIES
# ---------

set(XZ_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(XZ_LIB_DIR "${XZ_PROJECT_DIR}/lib")

add_subdirectory(src)
