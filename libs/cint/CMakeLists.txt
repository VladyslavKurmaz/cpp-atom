#
cmake_minimum_required( VERSION 2.6 )
project( z3d )

add_definitions(-D_WIN32_WINNT=0x0501)
add_definitions(-D__USE_W32_SOCKETS)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTI_THREADED ON)
#find_package( Boost 1.49.0 REQUIRED )


# Inc/Lib
#include_directories( "." )

include_directories( "$ENV{DEV_BOOST_1_50_0_INC}" )
link_directories( "$ENV{DEV_BOOST_1_50_0_LIB}" )

include_directories( "$ENV{DEV_LOKI_0_1_7_INC}" )

include_directories( "$ENV{DEV_CPP_NETLIB_0_9_4_INC}" )
link_directories( "C:\work\env\cpp-netlib-0.9.4\build-msvc10_x86\libs\network\src\Debug" )

link_directories( "$ENV{DEV_OPENSSL_1_0_1C_WIN32_LIB_VC_STATIC}" )



#include_directories( "$ENV{DEV_CINT_5_18_00_PATH_INC}" )
#link_directories( "$ENV{DEV_CINT_5_18_00_PATH_LIB}" )

add_executable( test "./test.cpp" )
target_link_libraries( test cppnetlib-client-connections.lib cppnetlib-server-parsers.lib cppnetlib-uri.lib )
target_link_libraries( test ssleay32MTd.lib libeay32MTd.lib )

set (BUILD_THIS 0)

if (BUILD_THIS)

# slot
add_executable( slot_01 "./libs/slot/test/slot_01.cpp" )


# slot
add_executable( scope_01 "./libs/scope/test/scope_01.cpp" )

# node
add_executable( node_01 "./libs/node/test/node_01.cpp" )
add_executable( node_02 "./libs/node/test/node_02.cpp" )
add_executable( node_03 "./libs/node/test/node_03.cpp" )
add_executable( node_04 "./libs/node/test/node_04.cpp" )
add_executable( node_05 "./libs/node/test/node_05.cpp" )

# type_name
add_executable( type_name_01 "./libs/type_name/test/type_name_01.cpp" )
add_executable( type_name_02 "./libs/type_name/test/type_name_02.cpp" )

# chain of responsibility
add_executable( chain_of_resp_01 "./libs/chain_of_resp/test/chain_of_resp_01.cpp" )
add_executable( chain_of_resp_02 "./libs/chain_of_resp/test/chain_of_resp_02.cpp" )
add_executable( chain_of_resp_03 "./libs/chain_of_resp/test/chain_of_resp_03.cpp" )

# gateway
add_executable( gateway_01 "./libs/gateway/test/gateway_01.cpp" )
add_executable( gateway_02 "./libs/gateway/test/gateway_02.cpp" )

# mstack
add_executable( mstack_01 "./libs/mstack/test/mstack_01.cpp" )
add_executable( mstack_02 "./libs/mstack/test/mstack_01.cpp" )

# cint
#add_executable( cint_01 "./libs/script/cint/test/cint_01.cpp" )
endif()
