#
#
#


set(imgdraw2d_FOUND FALSE)
if (imgdraw2d_INCLUDE_DIR AND (imgdraw2d_LIBRARY_RELEASE OR imgdraw2d_LIBRARY_DEBUG))
    ## paths configured
    set(imgdraw2d_FOUND TRUE)
    set(imgdraw2d_LIBRARY optimized ${imgdraw2d_LIBRARY_RELEASE} debug ${imgdraw2d_LIBRARY_DEBUG})
    return()
endif()


## Put here path to custom location

find_path(imgdraw2d_INCLUDE_DIR imgdraw2d/Drawer2D.h
    HINTS "$ENV{imgdraw2d_DIR} ${imgdraw2d_DIR}"
    PATHS ${CMAKE_MODULE_PATH}
    PATH_SUFFIXES include ../../include
)

FIND_LIBRARY(imgdraw2d_LIBRARY_RELEASE
  NAMES imgdraw2d
  HINTS "$ENV{imgdraw2d_DIR} ${imgdraw2d_DIR}"
  PATHS ${CMAKE_MODULE_PATH}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib ../../lib
)

FIND_LIBRARY(imgdraw2d_LIBRARY_DEBUG
  NAMES imgdraw2d_d
  HINTS "$ENV{imgdraw2d_DIR} ${imgdraw2d_DIR}"
  PATHS ${CMAKE_MODULE_PATH}
  NO_DEFAULT_PATH
  PATH_SUFFIXES lib ../../lib
)


if (imgdraw2d_INCLUDE_DIR AND (imgdraw2d_LIBRARY_RELEASE OR imgdraw2d_LIBRARY_DEBUG))
    set(imgdraw2d_FOUND TRUE)
    set(imgdraw2d_LIBRARY optimized ${imgdraw2d_LIBRARY_RELEASE} debug ${imgdraw2d_LIBRARY_DEBUG})
endif()


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
