cmake_minimum_required(VERSION 3.10)

project(
    nano_engine
    VERSION 0.1.0
    DESCRIPTION "Simple game engine"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NANO_ENGINE_BUILD_TESTS "Build tests about main library" ON)
option(NANO_ENGINE_BUILD_EXAMPLES "Build examples about main library" ON)

# Add extern folder for external libraries
add_subdirectory(extern)

# Add main library code
add_subdirectory(nano_engine)

# Add game code
add_subdirectory(games)

# Add test code
if(NANO_ENGINE_BUILD_TESTS)
    add_subdirectory(nano_engine.test)
endif()

# Add examples code
if(NANO_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


