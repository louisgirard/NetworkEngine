cmake_minimum_required(VERSION 3.10)

add_executable(client main.cpp)

target_link_libraries(client PRIVATE portable_socket)