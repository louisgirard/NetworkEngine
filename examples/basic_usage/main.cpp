#include <iostream>

#include <nano_engine/engine/engine.hpp>

#include <nano_engine/entities/actor.hpp>

#include <nano_engine/components/velocity.hpp>

#include <nano_engine/components/rigid_body.hpp>

#include <nano_engine/components/colliders/sphere_collider.hpp>
#include <nano_engine/components/colliders/box_collider.hpp>

int main(int argc, char* argv[])
{
    auto& engine = nano_engine::engine::Engine::Instance();
	engine.FrameLimiter(120);
    engine.ParseCommandeLine(argc, argv);
    engine.Startup();

	auto cube = std::make_shared<nano_engine::entities::Actor>(engine.GetWorld(), "floor");
	cube->GetPosition().y = -56.f;
	auto boxColl = cube->AddComponent<nano_engine::components::BoxCollider>(100.f, 100.f, 100.f);
	auto rb = cube->AddComponent<nano_engine::components::RigidBody>(boxColl, 0.0f, cube->GetPosition());

	auto sphere = std::make_shared<nano_engine::entities::Actor>(engine.GetWorld(), "sphere");
	sphere->GetPosition().y = 1000.f;
	auto sphereColl = sphere->AddComponent<nano_engine::components::SphereCollider>(1.0f);
	sphere->AddComponent<nano_engine::components::RigidBody>(sphereColl, 1.0f, sphere->GetPosition());

    engine.Run();
    return EXIT_SUCCESS;
}