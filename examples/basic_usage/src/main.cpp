#include <iostream>

#include <nano_engine/engine/engine.hpp>

#include <cube.hpp>
#include <sphere.hpp>

int main(int argc, char* argv[])
{
    auto& engine = nano_engine::engine::Engine::Instance();
	engine.FrameLimiter(120);
    engine.ParseCommandeLine(argc, argv);
    engine.Startup();

	auto cube = std::make_shared<Cube>(engine.GetWorld(), "floor", 100.0f);
	cube->SetPosition(0, -56.f, 0);

	auto sphere = std::make_shared<Sphere>(engine.GetWorld(), "sphere", 1.0f);
	sphere->SetPosition(0, 1000.0f, 0);

    engine.Run();
    return EXIT_SUCCESS;
}