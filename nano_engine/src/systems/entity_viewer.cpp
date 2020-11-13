#include <spdlog/spdlog.h>

#include <nano_engine/components/name.hpp>
#include <nano_engine/components/position.hpp>

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

			world.Registry().view<components::Name, components::Position>().each([](components::Name& name, components::Position& pos)
				{
					spdlog::info("Entity {} is at : X:{}/Y{}/Z{}", name, pos.x, pos.y, pos.z);
				});
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