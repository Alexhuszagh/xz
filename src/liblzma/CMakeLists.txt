#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

# LIBLZMA
# -------

# TODO: need check/Makefile.inc
# TODO: need lz/Makefile.inc
# TODO: need lzma/Makefile.inc
# TODO: need rangecoder/Makefile.inc
# TODO: need delta/Makefile.inc
# TODO: need simple/Makefile.inc

# TODO: enabled all encoders and decoders by default.
# TODO: need to check COND_MAIN_ENCODER

include_directories(
    check
    common
)

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

if(ENABLE_THREADS)
    list(APPEND LZMA_SOURCES
        ../common/tuklib_cpucores.c
        common/hardware_cputhreads.c
        common/outqueue.c
        common/stream_encoder_mt.c
    )
endif()

if(ENABLE_ENCODERS)
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

if(ENABLE_CRC32)
    if(ENABLE_SMALL)
        list(APPEND LZMA_SOURCES check/crc32_small.c)
    else()
        list(APPEND LZMA_SOURCES check/crc32_table.c)
    endif()

    if(ENABLE_ASSSEMBLER)
        list(APPEND LZMA_SOURCES check/crc32_x86.S)
    else()
        list(APPEND LZMA_SOURCES check/crc32_fast.c)
    endif()
endif()

if(ENABLE_CRC64)
    if(ENABLE_SMALL)
        list(APPEND LZMA_SOURCES check/crc64_small.c)
    else()
        list(APPEND LZMA_SOURCES check/crc64_table.c)
    endif()

    if(ENABLE_ASSSEMBLER)
        list(APPEND LZMA_SOURCES check/crc64_x86.S)
    else()
        list(APPEND LZMA_SOURCES check/crc64_fast.c)
    endif()
endif()

if(ENABLE_SHA256 AND EXTERNAL_SHA256)
    list(APPEND LZMA_SOURCES check/sha256.c)
endif()

# WINDOWS
# -------

if(WIN32 AND NOT BUILD_STATIC)
    # Add resource files only in shared builds
endif()

# LIBRARIES
# ---------

if(BUILD_STATIC)
    add_library(LibLzma
        STATIC
        ${LZMA_SOURCES}
    )
else()
    add_library(LibLzma
        SHARED
        ${LZMA_SOURCES}
    )
endif()

set_target_properties(LibLzma
    PROPERTIES OUTPUT_NAME lzma
)
