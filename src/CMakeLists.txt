#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see licenses/mit.md for more details.

# LIBRARIES
# ---------

add_subdirectory(liblzma)
if(BUILD_XZ)
    add_subdirectory(xzdec)
endif()
