#pragma once

#include <entt/entt.hpp>

#include <nano_engine/common.hpp>

#include <nano_engine/components/name.hpp>

namespace nano_engine::engine
{
	class World
	{
	public:
		explicit World(const std::string& name) {}
		~World()
		{
			m_registry.clear();
		}

		World(const World& other) = delete;
		World(World&& other) = delete;

		entt::registry& Registry()
		{
			return m_registry;
		}

		entt::entity CreateEntity(const std::string& name)
		{
			auto entity =  m_registry.create();
			AddComponent<components::Name>(entity, name);
			return entity;
		}
		
		template<typename Component_t, typename ...Args>
		Component_t& AddComponent(entt::entity entity, Args... args)
		{
			return m_registry.emplace<Component_t>(entity, std::forward<Args>(args)...);
		}

	private:
		entt::registry m_registry;
	};
}