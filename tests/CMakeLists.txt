#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

# LIBRARY
# -------

set(TEST_FILES
    test_check.c
    test_stream_flags.c
    test_filter_flags.c
    test_block_header.c
    test_index.c
    test_bcj_exact_size.c
)

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

foreach(test ${TEST_FILES})
    get_filename_component(test_name ${test} NAME_WE)
    add_executable(xz_${test_name} ${test})
    target_link_libraries(xz_${test_name} LibLzma)

    if(MSVC)
        set_target_properties(xz_test PROPERTIES
            COMPILE_OPTIONS "/EHsc"
        )
    endif()

    add_test(NAME xz_${test_name}
        COMMAND xz_${test_name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach(test)
