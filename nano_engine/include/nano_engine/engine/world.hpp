#pragma once

#include <nano_engine/common.hpp>

#include <nano_engine/components/name.hpp>

namespace nano_engine::engine
{
	class Entity;
	class WorldImpl;
	class World
	{
	public:
		explicit World(const std::string& name);
		~World();

		World(const World& other) = delete;
		World(World&& other) = delete;
		
		void CreateEntity(Entity* entity);
		void DestroyEntity(Entity* entity);

		std::vector<Entity*>& Entities();

	private:
		WorldImpl* m_impl;
	};
}