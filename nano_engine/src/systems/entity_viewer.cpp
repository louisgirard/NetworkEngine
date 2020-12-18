#include <spdlog/spdlog.h>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/systems/entity_viewer.hpp>

namespace nano_engine::systems
{
	class EntityViewerImpl
	{
	public:
		EntityViewerImpl()
		{

		}

		virtual ~EntityViewerImpl()
		{

		}

		EntityViewerImpl(const EntityViewerImpl& other) = delete;
		EntityViewerImpl(EntityViewerImpl&& other) = delete;

		void Update(std::chrono::microseconds deltaTime, engine::World& world)
		{
			m_totalMilli += deltaTime;
			if (m_totalMilli < 1s) return;

			for (auto it = world.Entities().begin(); it != world.Entities().end(); it++)
			{
				components::Name name = (*it)->GetName();
				components::Position pos = (*it)->GetPosition();
				spdlog::info("Entity {} is at : X:{}/Y{}/Z{}", name.name, pos.x, pos.y, pos.z);
			}
			m_totalMilli = 0ms;
		}

	private:
		std::chrono::microseconds m_totalMilli = 0ms;
	};

	EntityViewer::EntityViewer()
	{
		m_impl = std::make_unique<EntityViewerImpl>();
	}

	EntityViewer::~EntityViewer()
	{
		m_impl = nullptr;
	}

	void EntityViewer::Update(std::chrono::microseconds deltaTime, engine::World& world)
	{
		m_impl->Update(deltaTime, world);
	}
}