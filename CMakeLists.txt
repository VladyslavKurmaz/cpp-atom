#
cmake_minimum_required( VERSION 3.2.3 )
project( cpp-atom )

add_definitions(-DUNICODE -D_UNICODE)
#add_definitions("/EHsc")

# Inc/Lib
include_directories( "./" )

include_directories( "$ENV{DEV_BOOST-1.58.0_INC}" )
link_directories( "$ENV{DEV_BOOST-1.58.0_LIB}" )

include_directories( "$ENV{DEV_LOKI-0.1.7_INC}" )

# Real stuff
#add_subdirectory ( "./libs" )
#add_subdirectory ( "./console" )
add_subdirectory ( "./dev" )
