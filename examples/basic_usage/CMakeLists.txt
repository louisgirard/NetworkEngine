cmake_minimum_required(VERSION 3.10)

add_executable(basic_usage main.cpp)

target_link_libraries(basic_usage nano_engine)
target_link_libraries(basic_usage CONAN_PKG::bullet3)