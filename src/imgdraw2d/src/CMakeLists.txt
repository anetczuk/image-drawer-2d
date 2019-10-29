#
#
#


set( TARGET_NAME imgdraw2d )


set( PUBLIC_HEADERS "../include" )

include_directories( ${PUBLIC_HEADERS} )

set( EXT_LIBS ${PNG_LIBRARIES} ${png++_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} )

file(GLOB_RECURSE cpp_files *.cpp )
file(GLOB_RECURSE h_files ${PUBLIC_HEADERS}/*.h )


add_library( ${TARGET_NAME} SHARED ${cpp_files} )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )

set_target_properties( ${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${h_files}" )

install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         PUBLIC_HEADER DESTINATION include/${TARGET_NAME}
)

install( FILES Findimgdraw2d.cmake 
         DESTINATION cmake/Modules
         RENAME "Find${TARGET_NAME}.cmake"
)

option( INSTALL_TEST_UTILS "Install additional image test utils header (Boost)" OFF )
if ( INSTALL_TEST_UTILS )
    install( FILES ../tests/ImgTestUtils.h
             DESTINATION include/${TARGET_NAME}
    )
endif()
