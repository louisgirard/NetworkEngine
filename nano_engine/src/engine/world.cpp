#include <nano_engine/engine/entity.hpp>

#include <nano_engine/engine/world.hpp>

namespace nano_engine::engine
{
	class WorldImpl
	{
	public:
		explicit WorldImpl(const std::string& name)
		{

		}

		~WorldImpl()
		{
			m_entities.clear();
		}

		WorldImpl(const WorldImpl& other) = delete;
		WorldImpl(WorldImpl&& other) = delete;

		void CreateEntity(Entity* entity)
		{
			m_entities.push_back(entity);
		}

		void DestroyEntity(Entity* entity)
		{
			m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
		}

		std::vector<Entity*>& Entities()
		{
			return m_entities;
		}

	private:
		std::vector<Entity*> m_entities;
	};

	World::World(const std::string& name)
	{
		m_impl = new WorldImpl(name);
	}

	World::~World()
	{
		delete m_impl;
	}

	void World::CreateEntity(Entity* entity)
	{
		m_impl->CreateEntity(entity);
	}

	void World::DestroyEntity(Entity* entity)
	{
		m_impl->DestroyEntity(entity);
	}

	std::vector<Entity*>& World::Entities()
	{
		return m_impl->Entities();
	}
}