#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

# INCLUDES
# --------

include_directories(
    ${XZ_PROJECT_DIR}/src/common
    ${XZ_PROJECT_DIR}/src/liblzma/api
    ${XZ_PROJECT_DIR}/lib
)

# SOURCES
# -------

set(Xz_SOURCES
    args.c
    coder.c
    file_io.c
    hardware.c
    main.c
    message.c
    mytime.c
    options.c
    signals.c
    suffix.c
    util.c
    ../common/tuklib_open_stdxxx.c
    ../common/tuklib_progname.c
    ../common/tuklib_exit.c
    ../common/tuklib_mbstr_width.c
    ../common/tuklib_mbstr_fw.c
)

if(ENABLE_ENCODERS)
    list(APPEND Xz_SOURCES list.c)
endif()

if(WIN32)
    #list(APPEND Xz_SOURCES xz_w32res.rc)
endif()

# EXECUTABLE
# ----------

add_executable(Xz ${Xz_SOURCES})

set_target_properties(Xz
    PROPERTIES OUTPUT_NAME xz
)

# LINKING
# -------

target_link_libraries(Xz
    LibLzma
)

if(CMAKE_USE_PTHREADS_INIT)
    target_link_libraries(Xz pthread)
endif()

# FLAGS
# -----

if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DLZMA_API_STATIC)
endif()
