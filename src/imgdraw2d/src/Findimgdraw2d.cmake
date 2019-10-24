#
#
#


if (imgdraw2d_INCLUDE_DIR AND imgdraw2d_LIBRARY)
    ## paths configured
    set (imgdraw2d_FOUND TRUE)
    return()
endif ()


## Put here path to custom location

find_path(imgdraw2d_INCLUDE_DIR imgdraw2d/Drawer2D.h
    HINTS "$ENV{imgdraw2d_INCLUDE_PATH}"
    PATHS ${EXTERNAL_DIR}/include
)

FIND_LIBRARY(imgdraw2d_LIBRARY
  NAMES imgdraw2d
  PATHS ${EXTERNAL_DIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib
)

set(imgdraw2d_FOUND FALSE)
if (imgdraw2d_INCLUDE_DIR AND imgdraw2d_LIBRARY)
    set(imgdraw2d_FOUND TRUE)
endif()

## message( STATUS "imgdraw2d path: ${imgdraw2d_INCLUDE_DIR}" )


if (imgdraw2d_FOUND)
    if (NOT imgdraw2d_FIND_QUIETLY)
        message(STATUS "Found imgdraw2d include-dir path: ${imgdraw2d_INCLUDE_DIR}")
    endif (NOT imgdraw2d_FIND_QUIETLY)
else (imgdraw2d_FOUND)
    if (imgdraw2d_FIND_REQUIRED)
        message(FATAL_ERROR "could not find imgdraw2d")
    else()
        message( STATUS "imgdraw2d NOT REQUIRED" )
    endif (imgdraw2d_FIND_REQUIRED)
endif (imgdraw2d_FOUND)
