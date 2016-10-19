#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

# LIBRARIES
# ---------

set(XZ_COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/common")
set(XZ_LIBLZMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/liblzma")
set(XZ_API_DIR "${XZ_LIBLZMA_DIR}/api")

add_subdirectory(liblzma)
add_subdirectory(xzdec)

# TODO: check arguments and add subdirectories as needed
