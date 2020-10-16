cmake_minimum_required(VERSION 3.10)

add_executable(server main.cpp)

target_link_libraries(server PRIVATE portable_socket)