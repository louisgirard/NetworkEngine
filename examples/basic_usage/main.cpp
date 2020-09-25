#include <iostream>
#include <nano_engine/engine/engine.hpp>

int main(int argc, char* argv[])
{
    auto& engine = nano_engine::engine::Engine::Instance();
    engine.ParseCommandLine(argc, argv);
    engine.Startup();
    engine.Run();

    return EXIT_SUCCESS;
}