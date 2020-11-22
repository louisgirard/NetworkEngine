cmake_minimum_required(VERSION 3.10)

project(portable_socket VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(portable_socket)
add_subdirectory(client)
add_subdirectory(server)