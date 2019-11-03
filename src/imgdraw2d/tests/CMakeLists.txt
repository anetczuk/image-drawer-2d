#
#
#

if( NOT CMAKE_TESTING_ENABLED )  
    return()
endif()


set( TARGET_NAME imgdraw2d_tests )


include_directories( "../include" )


set( EXT_LIBS ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} imgdraw2d )

file(GLOB_RECURSE cpp_files *.cpp )


find_package(Eigen3 3.3 QUIET)

if ( ${EIGEN3_FOUND} )
    #old way: include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
    list(APPEND EXT_LIBS Eigen3::Eigen )
else()
    list(REMOVE_ITEM cpp_files "${CMAKE_CURRENT_SOURCE_DIR}/Drawer2DEigen_test.cpp" )
endif()


add_executable( ${TARGET_NAME} ${cpp_files} )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )

set( EXECUTABLE "./${TARGET_NAME}" )
generate_file( run_tests.sh.in )

add_test( NAME ${TARGET_NAME} COMMAND ${TARGET_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        

#TODO: copying data and scripts can be optimised by creating script from template with defined data directry

## copy run script
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/runTests.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

## copy data directory
file(COPY refimg DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
