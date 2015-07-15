#
cmake_minimum_required( VERSION 3.2.3 )
project( cpp-atom )

add_definitions(-DUNICODE -D_UNICODE)
#add_definitions("/EHsc")

# Inc/Lib
include_directories( "./" )

include_directories( "$ENV{BOOST_INCLUDE}" )
link_directories( "$ENV{BOOST_LIB}" )

include_directories( "$ENV{LOKI_INCLUDE}" )

# Real stuff
#add_subdirectory ( "./libs" )
#add_subdirectory ( "./console" )
add_subdirectory ( "./dev" )
