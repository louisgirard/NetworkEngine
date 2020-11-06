#include <iostream>
#include <nano_engine/engine/engine.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rigid_body.hpp>
#include <nano_engine/components/velocity.hpp>

#include <nano_engine/components/colliders/box_collider.hpp>
#include <nano_engine/components/colliders/sphere_collider.hpp>

int main(int argc, char* argv[])
{
    auto& engine = nano_engine::engine::Engine::Instance();
    engine.ParseCommandLine(argc, argv);
    engine.Startup();

    auto world = engine.GetWorld();

	auto cube = std::make_shared<nano_engine::engine::Entity>(*world.lock(), "Floor");
	cube->AddComponent<nano_engine::components::Velocity>(0.f, 0.f, 0.f);
	auto boxCollider = cube->AddComponent<nano_engine::components::BoxCollider>(100.f, 100.f, 100.f);
	cube->AddComponent<nano_engine::components::RigidBody>(boxCollider, 0.f, cube->Position());

	auto sphere = std::make_shared<nano_engine::engine::Entity>(*world.lock(), "Sphere");
	sphere->Position().y = 1000.f;
	sphere->AddComponent<nano_engine::components::Velocity>(0.f, 10.f, 0.f);
	auto sphereCollider = sphere->AddComponent<nano_engine::components::SphereCollider>(1);
	sphere->AddComponent<nano_engine::components::RigidBody>(sphereCollider, 1.f, sphere->Position());


    engine.Run();

    return EXIT_SUCCESS;
}