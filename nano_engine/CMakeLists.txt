cmake_minimum_required(VERSION 3.10)

set(NANO_ENGINE_HEADERS 
        include/nano_engine/common.hpp

        include/nano_engine/engine/engine.hpp

        include/nano_engine/systems/system.hpp
        include/nano_engine/systems/fps_counter.hpp 
        include/nano_engine/components/position.hpp 
        include/nano_engine/engine/world.hpp
        include/nano_engine/components/velocity.hpp
        include/nano_engine/systems/physic.hpp
        include/nano_engine/components/name.hpp
        include/nano_engine/systems/entity_viewer.hpp 
        include/nano_engine/components/rigid_body.hpp 
        include/nano_engine/components/colliders/capsule_collider.hpp
        "include/nano_engine/components/colliders/box_collider.hpp" 
        "include/nano_engine/components/colliders/sphere_collider.hpp" 
        "include/nano_engine/components/colliders/collider.hpp" 
        "include/nano_engine/serialization/memory_stream.hpp" 
        "include/nano_engine/serialization/endianess.hpp" 
        "include/nano_engine/serialization/input_memory_stream.hpp" 
        "include/nano_engine/serialization/output_memory_stream.hpp" 
        "include/nano_engine/components/rotation.hpp" 
        "include/nano_engine/systems/entity_serializer.hpp" 
        "include/nano_engine/engine/entity.hpp" 
        "include/nano_engine/components/scale.hpp" 
        "include/nano_engine/entities/actor.hpp" 
        "include/nano_engine/common/reference.hpp" 
        "include/nano_engine/common/assert.hpp" 
        "include/nano_engine/common/logger.hpp"
        "include/nano_engine/replication/linking_context.hpp"
        "include/nano_engine/replication/replication_manager.hpp"
        "include/nano_engine/components/replication_component.hpp" "include/nano_engine/replication/object_creation_registry.hpp")

set(NANO_ENGINE_SOURCES src/engine/engine.cpp
                        src/systems/fps_counter.cpp 
                        src/engine/world.cpp 
                        src/systems/physic.cpp
                        src/systems/entity_viewer.cpp 
                        "src/serialization/memory_stream.cpp"
                        "src/serialization/input_memory_stream.cpp" 
                        "src/serialization/output_memory_stream.cpp" 
                        "src/systems/entity_serializer.cpp" 
                        "src/engine/entity.cpp" 
                        "src/entities/actor.cpp"
                        "src/common/assert.cpp"
                        "src/replication/linking_context.cpp"
                        "src/replication/replication_manager.cpp" "src/replication/object_creation_registry.cpp")

add_library(nano_engine STATIC ${NANO_ENGINE_HEADERS} ${NANO_ENGINE_SOURCES})

target_include_directories(nano_engine INTERFACE include)
target_include_directories(nano_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(nano_engine PUBLIC CONAN_PKG::spdlog)
target_link_libraries(nano_engine PUBLIC CONAN_PKG::entt)
target_link_libraries(nano_engine PRIVATE CONAN_PKG::bullet3)