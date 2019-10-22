##
##
##


#find_package(PNG REQUIRED)
find_package(png++ REQUIRED)


find_package( Boost COMPONENTS filesystem system REQUIRED )


#include_directories(${OpenCV_INCLUDE_DIRS}
#    ${PNG_INCLUDE_DIR}
#    ${png++_INCLUDE_DIR}
#)


include_directories( 
    SYSTEM ${png++_INCLUDE_DIR}
)


add_subdirectories_recursive()
