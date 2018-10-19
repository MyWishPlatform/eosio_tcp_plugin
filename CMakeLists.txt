file(GLOB HEADERS "include/eosio/tcp_plugin/*.hpp")
add_library( tcp_plugin
             tcp_plugin.cpp
             server.cpp
             ${HEADERS} )

target_link_libraries( tcp_plugin chain_plugin appbase fc )
target_include_directories( tcp_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
