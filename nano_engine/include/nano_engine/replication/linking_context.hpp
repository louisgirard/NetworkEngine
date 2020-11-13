#pragma once

#include <map>

#include <nano_engine/common.hpp>
#include <nano_engine/engine/entity.hpp>

namespace nano_engine::replication
{
	class LinkingContext
	{
	public:
		static LinkingContext& Instance()
		{
			static LinkingContext context;
			return context;
		}

		engine::ObjectID_t GetObjectID(engine::Entity* entity) const;
		engine::Entity* GetEntity(const engine::ObjectID_t objectID) const;

		engine::ObjectID_t AddEntity(engine::Entity* entity);
		void RemoveEntity(engine::Entity* entity);
		void RemoveEntity(engine::ObjectID_t objectID);

	private:
		LinkingContext() {}

		engine::ObjectID_t m_nextObjectID = 0;

		std::map<engine::ObjectID_t, engine::Entity*> m_idToEntities;
		std::map<engine::Entity*, engine::ObjectID_t> m_entitiesToId;
	};
}