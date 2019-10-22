#
#
#


set( TARGET_NAME imgdraw2d )


include_directories( "../include" )


set( EXT_LIBS ${PNG_LIBRARIES} ${png++_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} )


file(GLOB_RECURSE cpp_files *.cpp )


add_library( ${TARGET_NAME} SHARED ${cpp_files} )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )
