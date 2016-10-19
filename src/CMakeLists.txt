#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

add_subdirectory(liblzma)
if(BUILD_XZ)
    add_subdirectory(xz)
    add_subdirectory(xzdec)
endif()

if(BUILD_LZMA_INFO)
    add_subdirectory(lzmainfo)
endif()

if(BUILD_SCRIPTS)
    add_subdirectory(scripts)
endif()

