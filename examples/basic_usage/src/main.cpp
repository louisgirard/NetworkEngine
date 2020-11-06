#include <iostream>
#include <nano_engine/engine/engine.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rigid_body.hpp>
#include <nano_engine/components/velocity.hpp>

#include <nano_engine/components/colliders/box_collider.hpp>
#include <nano_engine/components/colliders/sphere_collider.hpp>

#include <cube.hpp>
#include <sphere.hpp>

int main(int argc, char* argv[])
{
    auto& engine = nano_engine::engine::Engine::Instance();
    engine.ParseCommandLine(argc, argv);
    engine.Startup();

    auto world = engine.GetWorld();

	Cube cube(*world.lock(), "Floor", 10.f);
	cube.SetPosition(0.f, -22.f, 0.f);

	Sphere sphere(*world.lock(), "Sphere", 10.f);
	sphere.SetPosition(0.f, 1000.f, 0.f);
	//sphere.AddComponent<nano_engine::components::Velocity>(0.f, 10.f, 0.f);

    engine.Run();

    return EXIT_SUCCESS;
}