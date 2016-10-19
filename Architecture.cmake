#  :copyright: (c) 2012 Petroules Corporation. All rights reserved.
#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.
#
#   Code to detect architecture is modified from Petroules, the rest is
#   from The Regents of the University of California.
#       https://github.com/axr/solar-cmake

# ARCHITECTURE
# ------------

if(ARCH)
    return()
endif()

# COMPILER
# --------


set(ARCHITECTURE_CODE "
#if defined(__arm__) || defined(__TARGET_ARCH_ARM)
    #if defined(__ARM_ARCH_7__) \\
        || defined(__ARM_ARCH_7A__) \\
        || defined(__ARM_ARCH_7R__) \\
        || defined(__ARM_ARCH_7M__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 7)
        #error ARCH armv7
    #elif defined(__ARM_ARCH_6__) \\
        || defined(__ARM_ARCH_6J__) \\
        || defined(__ARM_ARCH_6T2__) \\
        || defined(__ARM_ARCH_6Z__) \\
        || defined(__ARM_ARCH_6K__) \\
        || defined(__ARM_ARCH_6ZK__) \\
        || defined(__ARM_ARCH_6M__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 6)
        #error ARCH armv6
    #elif defined(__ARM_ARCH_5TEJ__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 5)
        #error ARCH armv5
    #else
        #error ARCH arm
    #endif
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
    #error ARCH 32
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
    #error ARCH 64
#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
    #error ARCH 64
#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) \\
      || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC)  \\
      || defined(_M_MPPC) || defined(_M_PPC)
    #if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
        #error ARCH 64
    #else
        #error ARCH 32
    #endif
#endif
#error ARCH unknown
"
)

if(APPLE)
    # OSX
    if(CMAKE_OSX_ARCHITECTURES MATCHES "x86_64;i386")
        set(ARCH 96)
    elseif(CMAKE_OSX_ARCHITECTURES MATCHES "i386")
        set(ARCH 32)
    elseif(CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
        set(ARCH 64)
    endif()
elseif(UNIX_LIKE)
    # Linux, other *Nix
    if(CMAKE_CXX_FLAGS MATCHES "-m32")
        set(ARCH 32)
    elseif(CMAKE_CXX_FLAGS MATCHES "-m64")
        set(ARCH 64)
    endif()
endif()

if(NOT ARCH)
    # Compile program and read error for Architecture
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/arch.cpp" "${ARCHITECTURE_CODE}")
    # Not detecting the proper environment
    enable_language(CXX)
    try_run(
        run_result_unused
        compile_result_unused
        "${CMAKE_BINARY_DIR}"
        "${CMAKE_BINARY_DIR}/arch.cpp"
        COMPILE_OUTPUT_VARIABLE CMAKE_ARCH
    )
    string(REGEX MATCH "ARCH ([a-zA-Z0-9_]+)" CMAKE_ARCH "${CMAKE_ARCH}")
    string(REPLACE "ARCH " "" ARCH "${CMAKE_ARCH}")
endif()

if(NOT ARCH OR ARCH MATCHES "unknown")
    message(FATAL_ERROR "Could not detect architecture.")
else()
    message("Architecture detected and is ${ARCH}")
endif()
