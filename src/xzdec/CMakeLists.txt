#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

# TODO: need to if/else this whole thing for LZMADEC, XZDEC

# XZDEC
# -----

set(XZDEC_SOURCES
    xzdec.c
    ../common/tuklib_progname.c
    ../common/tuklib_exit.c
)

set(LZMADEC_SOURCES ${XZDEC_SOURCES})

if(WIN32)
    list(APPEND XZDEC_SOURCES xzdec_w32res.rc)
    list(APPEND LZMADEC_SOURCES lzmadec_w32res.rc)
endif()

include_directories(
    ${XZ_PROJECT_DIR}/src/common
    ${XZ_PROJECT_DIR}/src/liblzma
    ${XZ_PROJECT_DIR}/src/liblzma/api
)

# EXECUTABLES
# -----------

add_executable(XzDec ${XZDEC_SOURCES})
add_executable(LzmaDec ${LZMADEC_SOURCES})

set_target_properties(XzDec
    PROPERTIES OUTPUT_NAME xzdec
)
set_target_properties(LzmaDec
    PROPERTIES OUTPUT_NAME lzmadec
)

# LINKING
# -------

target_link_libraries(XzDec LibLzma)
target_link_libraries(LzmaDec LibLzma)

# FLAGS
# -----

target_compile_definitions(XzDec PUBLIC -DTUKLIB_GETTEXT=0)
target_compile_definitions(LzmaDec PUBLIC -DLZMADEC)

if(BUILD_STATIC)
    add_definitions(-DLZMA_API_STATIC)
endif()

#if COND_GNULIB
#xzdec_LDADD += $(top_builddir)/lib/libgnu.a
#endif
# xzdec_LDADD += $(LTLIBINTL)

