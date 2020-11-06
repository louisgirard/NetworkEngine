#include <entt/entt.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/engine/world.hpp>

namespace nano_engine::engine
{
	class WorldImpl
	{
	public:
		explicit WorldImpl(const std::string& name) {}
		~WorldImpl()
		{
			m_registry.clear();
		}

		WorldImpl(const WorldImpl& other) = delete;
		WorldImpl(WorldImpl&& other) = delete;

		entt::registry& Registry()
		{
			return m_registry;
		}

		uint32_t CreateEntity()
		{
			return static_cast<uint32_t>(m_registry.create());
		}

	private:
		entt::registry m_registry;
	};

	World::World(const std::string& name) : m_impl(std::make_unique<WorldImpl>(name))
	{
	}

	World::~World()
	{
		m_impl = nullptr;
	}

	entt::registry& World::Registry()
	{
		return m_impl->Registry();
	}

	uint32_t World::CreateEntity()
	{
		return m_impl->CreateEntity();
	}
}