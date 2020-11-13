#pragma once

#include <nano_engine/common.hpp>

#include <entt/entt.hpp>

#include <nano_engine/components/name.hpp>

namespace nano_engine::engine
{
	using EntityID_t = uint32_t;
	using EntityRegistry = entt::registry;

	class Entity;
	class WorldImpl;
	class World
	{
	public:
		explicit World(const std::string& name);
		~World();

		World(const World& other) = delete;
		World(World&& other) = delete;

		EntityRegistry& Registry();
		
		EntityID_t CreateEntity();
		void DestroyEntity(EntityID_t id);

	private:
		WorldImpl* m_impl;
	};
}