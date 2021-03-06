#include <atomic>
#include <vector>

#include <spdlog/spdlog.h>

#include <nano_engine/systems/system.hpp>

#include <nano_engine/systems/fps_counter.hpp>
#include <nano_engine/systems/entity_viewer.hpp>
#include <nano_engine/systems/physic.hpp>

#include <nano_engine/replication/replication_manager.hpp>

#include <nano_engine/engine/engine.hpp>
#include <nano_engine/engine/world.hpp>
#include <nano_engine/engine/entity.hpp>

#include <btBulletDynamicsCommon.h>

#include <cxxopts.hpp> 

namespace nano_engine::engine
{
	class EngineImpl
	{
	public:
		EngineImpl(uint32_t frameLimit = 0)
		{
			m_world = std::make_shared<World>("nano_engine");

			FrameLimiter(frameLimit);
		}

		~EngineImpl()
		{
			Stop();
		}

		void ParseCommandeLine(const std::string& gameName, int argc, char* argv[])
		{
			cxxopts::Options options(gameName, "CLI of a game built with nano_engine");
			options.add_options()
				("s,server", "Run in server mode")
				("i, ip", "IP on which to listen or connect", cxxopts::value<std::string>()->default_value("127.0.0.1"))
				("p, port", "Port on which to listen or connect", cxxopts::value<uint16_t>()->default_value("40'000"));
		
			auto result = options.parse(argc, argv);
			m_serverMode = (result.count("server") > 0);

			m_ip = result["ip"].as<std::string>();
			m_port = result["port"].as<uint16_t>();
		}

		void Startup()
		{
			spdlog::set_level(spdlog::level::debug);

			m_systems.emplace_back(std::make_unique<systems::FPSCounter>());
			m_systems.emplace_back(std::make_unique<systems::Physics>(0, -9.8f, 0));
			m_systems.emplace_back(std::make_unique<systems::EntityViewer>());
			m_systems.emplace_back(std::make_unique<replication::ReplicationManager>(m_serverMode, m_ip, m_port));
		}

		void Run()
		{
			if (!m_stop) return;

			m_stop = false;

			std::chrono::microseconds deltaTime(0);
			while (!m_stop)
			{
				// BeginFrame on all the systems
				for (auto& system : m_systems)
				{
					system->BeginFrame();
				}

				auto start = std::chrono::high_resolution_clock::now();

				// Update all the systems
				for (auto& system : m_systems)
				{
					system->Update(deltaTime, *m_world);
				}
				
				// EndFrame on all the systems
				for (auto& system : m_systems)
				{
					system->EndFrame();
				}

				auto end = std::chrono::high_resolution_clock::now();
				deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

				if (m_fixedFrameRate != 0)
				{
					while (deltaTime < m_frameDuration)
					{
						std::this_thread::yield();
						end = std::chrono::high_resolution_clock::now();
						deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
					}
				}
			}
		}

		void FrameLimiter(uint32_t frameRate)
		{
			m_fixedFrameRate = 0;
			if (frameRate == 0) return;

			m_fixedFrameRate = frameRate;
			m_frameDuration = 1'000'000us / m_fixedFrameRate;
		}

		void Stop()
		{
			m_stop = true;
		}

		std::shared_ptr<World> GetWorld()
		{
			return m_world;
		}

		bool IsServer() const
		{
			return m_serverMode;
		}

	private:
		std::atomic_bool m_stop = true;

		std::vector<std::unique_ptr<systems::ISystem>> m_systems;

		std::shared_ptr<World> m_world;

		uint32_t m_fixedFrameRate = 0;

		std::chrono::microseconds m_frameDuration = 0us;

		bool m_serverMode = false;
		std::string m_ip = "";
		uint16_t m_port = 0;
	};

	Engine::Engine()
	{
		m_impl = std::make_unique<EngineImpl>();
	}

	Engine::~Engine()
	{
		m_impl = nullptr;
	}

	void Engine::ParseCommandeLine(const std::string& gameName, int argc, char* argv[])
	{
		m_impl->ParseCommandeLine(gameName, argc, argv);
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

	void Engine::FrameLimiter(uint32_t frameRate)
	{
		m_impl->FrameLimiter(frameRate);
	}

	std::shared_ptr<World> Engine::GetWorld()
	{
		return m_impl->GetWorld();
	}

	bool Engine::IsServer() const
	{
		return m_impl->IsServer();
	}
}