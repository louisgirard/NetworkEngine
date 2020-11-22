#include <nano_engine/replication/linking_context.hpp>
#include <nano_engine/engine/entity.hpp>

namespace nano_engine::replication
{
	uint64_t LinkingContext::GetObjectID(engine::Entity* entity) const
	{
		if (auto it = m_entitiesToId.find(entity); it != m_entitiesToId.end())
		{
			return it->second;
		}
		
		return 0;
	}

	engine::Entity* LinkingContext::GetEntity(const uint64_t objectID) const
	{
		if (auto it = m_idToEntities.find(objectID); it != m_idToEntities.end())
		{
			return it->second;
		}

		return nullptr;
	}

	uint64_t LinkingContext::AddEntity(engine::Entity* entity)
	{
		if (entity == nullptr) return 0;

		auto objectID = ++m_nextObjectID;
		m_idToEntities.insert(std::make_pair(objectID, entity));
		m_entitiesToId.insert(std::make_pair(entity, objectID));

		return objectID;
	}

	void LinkingContext::RemoveEntity(engine::Entity* entity)
	{
		if (entity == nullptr) return;

		if (auto it = m_entitiesToId.find(entity); it != m_entitiesToId.end())
		{
			auto objectID = it->second;
			m_entitiesToId.erase(it);
			m_idToEntities.erase(objectID);
		}
	}

	void LinkingContext::RemoveEntity(uint64_t objectID)
	{
		if (objectID == 0) return;

		if (auto it = m_idToEntities.find(objectID); it != m_idToEntities.end())
		{
			auto entity = it->second;
			m_idToEntities.erase(it);
			m_entitiesToId.erase(entity);
		}
	}
}