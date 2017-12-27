#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

# LIBLZMA
# -------

# ENABLE_SYMBOL_VERSIONS

# INCLUDE
# -------

include_directories(
    ${XZ_PROJECT_DIR}/src/common
    ${XZ_PROJECT_DIR}/src/liblzma/api
    ${XZ_PROJECT_DIR}/src/liblzma/common
    ${XZ_PROJECT_DIR}/src/liblzma/check
    ${XZ_PROJECT_DIR}/src/liblzma/lz
    ${XZ_PROJECT_DIR}/src/liblzma/lzma
    ${XZ_PROJECT_DIR}/src/liblzma/delta
    ${XZ_PROJECT_DIR}/src/liblzma/rangecoder
    ${XZ_PROJECT_DIR}/src/liblzma/simple
)

# SOURCES
# -------

set(LZMA_SOURCES
    ../common/tuklib_physmem.c
    check/check.c
    common/common.c
    common/block_util.c
    common/easy_preset.c
    common/filter_common.c
    common/hardware_physmem.c
    common/index.c
    common/stream_flags_common.c
    common/vli_size.c
)

# THREADING

if(ENABLE_THREADS)
    list(APPEND LZMA_SOURCES
        ../common/tuklib_cpucores.c
        common/hardware_cputhreads.c
        common/outqueue.c
        common/stream_encoder_mt.c
    )
endif()

# CHECKS

if(ENABLE_CRC32)
    if(ENABLE_SMALL)
        list(APPEND LZMA_SOURCES check/crc32_small.c)
    else()
        list(APPEND LZMA_SOURCES check/crc32_table.c)

         if(ENABLE_ASSSEMBLER AND ARCH EQUAL 32)
            list(APPEND LZMA_SOURCES check/crc32_x86.S)
        else()
            list(APPEND LZMA_SOURCES check/crc32_fast.c)
        endif()
    endif()
endif()

if(ENABLE_CRC64)
    if(ENABLE_SMALL)
        list(APPEND LZMA_SOURCES check/crc64_small.c)
    else()
        list(APPEND LZMA_SOURCES check/crc64_table.c)

        if(ENABLE_ASSSEMBLER AND ARCH EQUAL 32)
            list(APPEND LZMA_SOURCES check/crc64_x86.S)
        else()
            list(APPEND LZMA_SOURCES check/crc64_fast.c)
        endif()
    endif()
endif()

if(ENABLE_SHA256 AND EXTERNAL_SHA256)
    list(APPEND LZMA_SOURCES check/sha256.c)
endif()

# FILTERS

if(ENABLE_ENCODERS)
    list(APPEND LZMA_SOURCES
    common/alone_encoder.c
    common/block_buffer_encoder.c
    common/block_encoder.c
    common/block_header_encoder.c
    common/easy_buffer_encoder.c
    common/easy_encoder.c
    common/easy_encoder_memusage.c
    common/filter_buffer_encoder.c
    common/filter_encoder.c
    common/filter_flags_encoder.c
    common/index_encoder.c
    common/stream_buffer_encoder.c
    common/stream_encoder.c
    common/stream_flags_encoder.c
    common/vli_encoder.c
    )
endif()

if(ENABLE_DECODERS)
    list(APPEND LZMA_SOURCES
        common/alone_decoder.c
        common/auto_decoder.c
        common/block_buffer_decoder.c
        common/block_decoder.c
        common/block_header_decoder.c
        common/easy_decoder_memusage.c
        common/filter_buffer_decoder.c
        common/filter_decoder.c
        common/filter_flags_decoder.c
        common/index_decoder.c
        common/index_hash.c
        common/stream_buffer_decoder.c
        common/stream_decoder.c
        common/stream_flags_decoder.c
        common/vli_decoder.c
    )
endif()

if(ENABLE_LZ)
    list(APPEND LZMA_SOURCES
        lz/lz_encoder.c
        lz/lz_encoder_mf.c
        lz/lz_decoder.c
    )
endif()

if(ENABLE_LZMA1)
    list(APPEND LZMA_SOURCES
        lzma/lzma_encoder.c
        lzma/lzma_encoder_optimum_fast.c
        lzma/lzma_encoder_optimum_normal.c
        lzma/lzma_decoder.c
        lzma/lzma_encoder_presets.c
        rangecoder/price_table.c
    )
    if (NOT ENABLE_SMALL)
        list(APPEND LZMA_SOURCES lzma/fastpos_table.c)
    endif()
endif()

if(ENABLE_LZMA2)
    list(APPEND LZMA_SOURCES
        lzma/lzma2_encoder.c
        lzma/lzma2_decoder.c
    )
endif()

if(ENABLE_DELTA)
    list(APPEND LZMA_SOURCES
        delta/delta_common.c
        delta/delta_encoder.c
        delta/delta_decoder.c
    )
endif()

if(ENABLE_SIMPLE)
    list(APPEND LZMA_SOURCES
        simple/simple_coder.c
        simple/simple_encoder.c
        simple/simple_decoder.c
    )
endif()

if(ENABLE_X86)
    list(APPEND LZMA_SOURCES simple/x86.c)
endif()

if(ENABLE_POWERPC)
    list(APPEND LZMA_SOURCES simple/powerpc.c)
endif()

if(ENABLE_IA64)
    list(APPEND LZMA_SOURCES simple/ia64.c)
endif()

if(ENABLE_ARM)
    list(APPEND LZMA_SOURCES simple/arm.c)
endif()

if(ENABLE_ARMTHUMB)
    list(APPEND LZMA_SOURCES simple/armthumb.c)
endif()

if(ENABLE_SPARC)
    list(APPEND LZMA_SOURCES simple/sparc.c)
endif()

# FLAGS
# -----

set(LZMA_LINK_FLAGS -no-undefined -version-info 8:99:3)

# WINDOWS
# -------

if(WIN32 AND NOT BUILD_STATIC)
    # Add resource files only in shared builds
    list(APPEND LZMA_SOURCES liblzma_w32res.rc)
    #list(APPEND LZMA_LINK_FLAGS) TODO: Def file flags?
endif()

# FLAGS
# -----

add_definitions(-DTUKLIB_SYMBOL_PREFIX=lzma_)

# LIBRARIES
# ---------

add_library(LibLzma
    ${LZMA_SOURCES}
)

set_target_properties(LibLzma
    PROPERTIES OUTPUT_NAME lzma
    LINK_FLAGS ${LZMA_LINK_FLAGS}
)
