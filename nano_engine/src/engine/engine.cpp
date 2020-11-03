#include <atomic>
#include <vector>

#include <spdlog/spdlog.h>

#include <nano_engine/systems/system.hpp>
#include <nano_engine/systems/fps_counter.hpp>
#include <nano_engine/systems/physic.hpp>
#include <nano_engine/systems/entity_viewer.hpp>
#include <nano_engine/systems/entity_serializer.hpp>

#include <nano_engine/engine/engine.hpp>
#include <nano_engine/engine/world.hpp>

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rotation.hpp>
#include <nano_engine/components/rigid_body.hpp>
#include <nano_engine/components/velocity.hpp>

#include <nano_engine/components/colliders/box_collider.hpp>
#include <nano_engine/components/colliders/sphere_collider.hpp>

#include <btBulletDynamicsCommon.h>

namespace nano_engine::engine
{
	class EngineImpl
	{
	public:
		EngineImpl() : m_world(std::make_shared<World>("nano-engine"))
		{
			auto cube = m_world->CreateEntity("cube");
			auto cubePos = m_world->AddComponent<components::Position>(cube, 0.f, 0.f, 0.f);
			m_world->AddComponent<components::Velocity>(cube, 0.f, 0.f, 0.f);
			m_world->AddComponent<components::Rotation>(cube, 0.f, 0.f, 0.f, 0.f);

			auto boxCollider = m_world->AddComponent<components::BoxCollider>(cube, 100.f, 100.f, 100.f);
			m_world->AddComponent<components::RigidBody>(cube, boxCollider, 0.f, cubePos);

			auto sphere = m_world->CreateEntity("sphere");
			auto spherePos = m_world->AddComponent<components::Position>(sphere, 0.f, 1000.f, 0.f);
			m_world->AddComponent<components::Velocity>(sphere, 0.f, 10.f, 0.f);
			m_world->AddComponent<components::Rotation>(sphere, 0.f, 0.f, 0.f, 0.f);

			auto sphereCollider = m_world->AddComponent<components::SphereCollider>(sphere, 1);
			m_world->AddComponent<components::RigidBody>(sphere, sphereCollider, 1.f, spherePos);
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
			spdlog::set_level(spdlog::level::debug);
			m_systems.emplace_back(std::make_unique<systems::FPSCounter>());
			m_systems.emplace_back(std::make_unique<systems::Physics>(0.f, -9.8f, 0.f));
			m_systems.emplace_back(std::make_unique<systems::EntityViewer>());
			m_systems.emplace_back(std::make_unique<systems::EntitySerializer>());
		}

		void Run()
		{
			if (!m_stop) return;

			m_stop = false;

			std::chrono::milliseconds deltaTime(0);
			while (!m_stop)
			{
				for (auto& system : m_systems)
				{
					system->BeginFrame();
				}

				auto start = std::chrono::high_resolution_clock::now();
				
				for (auto& system : m_systems)
				{
					system->Update(deltaTime, *m_world);
				}

				std::this_thread::sleep_for(1ms);

				for (auto& system : m_systems)
				{
					system->EndFrame();
				}

				auto end = std::chrono::high_resolution_clock::now();
				deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			}
		}

		void Stop()
		{
			m_stop = true;
		}

		std::weak_ptr<World> GetWorld()
		{
			return m_world;
		}

	private:
		std::atomic_bool m_stop = true;

		std::vector<std::unique_ptr<systems::ISystem>> m_systems;

		std::shared_ptr<World> m_world;
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

	std::weak_ptr<World> Engine::GetWorld()
	{
		return m_impl->GetWorld();
	}
}