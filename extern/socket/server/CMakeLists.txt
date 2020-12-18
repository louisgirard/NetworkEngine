cmake_minimum_required(VERSION 3.10)

add_executable(server_tcp main_tcp.cpp)
add_executable(server_udp main_udp.cpp)

target_link_libraries(server_tcp PRIVATE portable_socket)
target_link_libraries(server_udp PRIVATE portable_socket)