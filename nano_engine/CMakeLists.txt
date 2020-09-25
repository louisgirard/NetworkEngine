cmake_minimum_required(VERSION 3.10)

set(NANO_ENGINE_HEADERS
    include/nano_engine/common.hpp
    include/nano_engine/engine/engine.hpp
    include/nano_engine/systems/system.hpp
    include/nano_engine/systems/fps_counter.hpp
)

set(NANO_ENGINE_SOURCES
    src/engine/engine.cpp
    src/systems/fps_counter.cpp
)

add_library(nano_engine STATIC ${NANO_ENGINE_HEADERS} ${NANO_ENGINE_SOURCES})
target_include_directories(nano_engine INTERFACE include)
target_include_directories(nano_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)