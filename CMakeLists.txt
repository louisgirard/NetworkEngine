cmake_minimum_required(VERSION 3.10)

project(
    nano_engine
    VERSION 0.1.0
    DESCRIPTION "A simple engine to demonstrate network"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NANO_ENGINE_BUILD_TESTS "Build tests alongside the main library" ON)
option(NANO_ENGINE_BUILD_EXAMPLES "Build examples alongside the main library" ON)

# Setup CONAN package manager
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include("${CMAKE_BINARY_DIR}/conan.cmake")

conan_cmake_run(REQUIRES spdlog/1.8.0
                         bullet3/2.89
                         cxxopts/2.2.1
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

# Add extern folder for external librairies
add_subdirectory(extern)

# Add main library code
add_subdirectory(nano_engine)

#Add game code
add_subdirectory(games)

#Add test code
if(NANO_ENGINE_BUILD_TESTS)
    add_subdirectory(nano_engine.test)
endif()

#Add examples code
if(NANO_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()