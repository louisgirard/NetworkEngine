#pragma once

#include <map>

#include <nano_engine/common.hpp>

namespace nano_engine::engine
{
	class Entity;
}

namespace nano_engine::replication
{
	class LinkingContext
	{
	public:
		LinkingContext() {}

		uint64_t GetObjectID(engine::Entity* entity) const;
		engine::Entity* GetEntity(const uint64_t objectID) const;

		uint64_t AddEntity(engine::Entity* entity);
		void RemoveEntity(engine::Entity* entity);
		void RemoveEntity(uint64_t objectID);

	private:
		uint64_t m_nextObjectID = 0;

		std::map<uint64_t, engine::Entity*> m_idToEntities;
		std::map<engine::Entity*, uint64_t> m_entitiesToId;
	};
}