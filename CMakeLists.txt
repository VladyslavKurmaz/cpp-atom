#
cmake_minimum_required( VERSION 2.6 )
project( cpp-atom )

#add_definitions(-DUNICODE -D_UNICODE)


# Inc/Lib
include_directories( "./" )

include_directories( "$ENV{DPM_BOOST-1.53.0_INC}" )
link_directories( "$ENV{DPM_BOOST-1.53.0_LIB}" )

include_directories( "$ENV{DPM_LOKI-0.1.7_INC}" )

# Real stuff
#add_subdirectory ( "./libs" )
#add_subdirectory ( "./atom/cons" )
add_subdirectory ( "./atom/dpm" )
