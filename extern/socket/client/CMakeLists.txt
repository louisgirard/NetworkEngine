cmake_minimum_required(VERSION 3.10)

add_executable(client_tcp main_tcp.cpp)
add_executable(client_udp main_udp.cpp)

target_link_libraries(client_tcp PRIVATE portable_socket)
target_link_libraries(client_udp PRIVATE portable_socket)