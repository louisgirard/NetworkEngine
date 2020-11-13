#include <entt/entt.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/engine/world.hpp>

namespace nano_engine::engine
{
	using EntityIDObscureType = entt::entity;

	class WorldImpl
	{
	public:
		explicit WorldImpl(const std::string& name)
		{

		}

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

		EntityID_t CreateEntity()
		{
			return static_cast<uint32_t>(m_registry.create());
		}

		void DestroyEntity(EntityID_t id)
		{
			m_registry.destroy(static_cast<EntityIDObscureType>(id));
		}

	private:
		entt::registry m_registry;
	};

	World::World(const std::string& name)
	{
		m_impl = new WorldImpl(name);
	}

	World::~World()
	{
		delete m_impl;
	}

	entt::registry& World::Registry()
	{
		return m_impl->Registry();
	}

	EntityID_t World::CreateEntity()
	{
		return m_impl->CreateEntity();
	}

	void World::DestroyEntity(EntityID_t id)
	{
		m_impl->DestroyEntity(id);
	}
}