#
cmake_minimum_required( VERSION 2.6 )
project( cpp-atom )

add_definitions(-D_WIN32_WINNT=0x0501)
add_definitions(-D__USE_W32_SOCKETS)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTI_THREADED ON)
#find_package( Boost 1.51.0 REQUIRED )


# Inc/Lib
include_directories( "./" )

include_directories( "$ENV{DPM_BOOST_1_49_0_INC}" )
link_directories( "$ENV{DPM_BOOST_1_49_0_LIB}" )

include_directories( "$ENV{DPM_LOKI_0_1_7_INC}" )

# Real stuff
add_subdirectory ( "./libs" )
