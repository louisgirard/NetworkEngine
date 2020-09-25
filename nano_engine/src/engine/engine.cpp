#include <atomic>
#include <vector>

#include <nano_engine/systems/system.hpp>
#include <nano_engine/systems/fps_counter.hpp>
#include <nano_engine/engine/engine.hpp>

namespace nano_engine::engine
{
	class EngineImpl
	{
	public:
		EngineImpl()
		{

		}

		~EngineImpl()
		{
			Stop(); 
		}

		void ParseCommandLine(int argc, char* argv[])
		{

		}

		void Startup()
		{
			m_systems.emplace_back(std::make_unique<systems::FPSCounter>());
		}

		void Run()
		{
			if (!m_stop) return;

			m_stop = false;

			std::chrono::milliseconds deltaTime(0);
			while (!m_stop)
			{
				auto start = std::chrono::high_resolution_clock::now();
				
				for (auto& system : m_systems)
				{
					system->Update(deltaTime);
				}

				std::this_thread::sleep_for(1ms);

				auto end = std::chrono::high_resolution_clock::now();
				deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			}
		}

		void Stop()
		{
			m_stop = true;
		}

	private:
		std::atomic_bool m_stop = true;

		std::vector<std::unique_ptr<systems::ISystem>> m_systems;
	};

	Engine::Engine() : m_impl(std::make_unique<EngineImpl>())
	{

	}

	Engine::~Engine()
	{
		m_impl = nullptr;
	}

	void Engine::ParseCommandLine(int argc, char* argv[])
	{
		m_impl->ParseCommandLine(argc, argv);
	}

	void Engine::Startup()
	{
		m_impl->Startup();
	}

	void Engine::Run()
	{
		m_impl->Run();
	}

	void Engine::Stop()
	{
		m_impl->Stop();
	}
}