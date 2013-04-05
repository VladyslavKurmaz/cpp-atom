#
cmake_minimum_required( VERSION 2.6 )
project( cpp-atom )

# Inc/Lib
include_directories( "./" )

include_directories( "$ENV{DPM_BOOST_1_53_0_INC}" )
link_directories( "$ENV{DPM_BOOST_1_53_0_LIB}" )

include_directories( "$ENV{DPM_LOKI_0_1_7_INC}" )

# Real stuff
#add_subdirectory ( "./libs" )
add_subdirectory ( "./atom/cons" )
