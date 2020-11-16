cmake_minimum_required(VERSION 3.10)

add_executable(basic_usage "src/main.cpp"
"include/cube.hpp"
"include/sphere.hpp"
"src/cube.cpp"
"src/sphere.cpp")
target_include_directories(basic_usage PUBLIC include)
target_link_libraries(basic_usage nano_engine)
target_link_libraries(basic_usage CONAN_PKG::bullet3)