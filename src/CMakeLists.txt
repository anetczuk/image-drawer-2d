##
##
##

cmake_minimum_required(VERSION 3.1.1)


if( "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}" )
    message( FATAL_ERROR "Building in source is forbidden. Change output directory.")
endif()


project( memory-performance )


set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )


include( CMakeCompileFlags.txt )

include( CMakeCoverage.txt )

include( CMakeSanitizer.txt )

include( CMakeUtils.txt )


##
## enable CTest utility
##
enable_testing()



## ================= build =================



include_directories( . )


if( CMAKE_TESTING_ENABLED )   
    #include_directories( test_utils )
    
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    
endif()


add_subdirectories_recursive()
