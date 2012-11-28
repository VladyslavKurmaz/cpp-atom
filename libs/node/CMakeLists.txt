set (BUILD_THIS 0)


# chain of responsibility
add_executable( chain_of_resp_01 "./test/chain_of_resp_01.cpp" )
add_executable( chain_of_resp_02 "./test/chain_of_resp_02.cpp" )
add_executable( chain_of_resp_03 "./test/chain_of_resp_03.cpp" )

# gateway
add_executable( gateway_01 "./test/gateway_01.cpp" )
add_executable( gateway_02 "./test/gateway_02.cpp" )

# mstack
add_executable( mstack_01 "./test/mstack_01.cpp" )
add_executable( mstack_02 "./test/mstack_01.cpp" )

# node
add_executable( node_01 "./test/node_01.cpp" )
add_executable( node_02 "./test/node_02.cpp" )
add_executable( node_03 "./test/node_03.cpp" )
add_executable( node_04 "./test/node_04.cpp" )
add_executable( node_05 "./test/node_05.cpp" )

# scope
add_executable( scope_01 "./test/scope_01.cpp" )

# slot
add_executable( slot_01 "./test/slot_01.cpp" )

if (BUILD_THIS)
endif()





