#include <iostream>
#include <nano_engine/engine.hpp>

int main(int argc, char* argv[])
{
    auto& engine = nano_engine::Engine::Instance();
    engine.ParseCommandLine(argc, argv);
    engine.Run();

    return EXIT_SUCCESS;
}