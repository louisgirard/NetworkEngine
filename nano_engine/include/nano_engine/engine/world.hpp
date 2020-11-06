#pragma once

#include <nano_engine/common.hpp>

#include <entt/entt.hpp>

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

		entt::registry& Registry();

		uint32_t CreateEntity();

	private:
		std::unique_ptr<WorldImpl> m_impl;
	};
}