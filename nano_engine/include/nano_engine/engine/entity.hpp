#pragma once

#include <type_traits>
#include <cstdint>

#include <entt/entt.hpp>

#include <nano_engine/common.hpp>

#include <nano_engine/components/name.hpp>
#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rotation.hpp>
#include <nano_engine/components/scale.hpp>

#include <nano_engine/engine/world.hpp>

#include <nano_engine/serialization/input_memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

namespace nano_engine::engine
{
	class Entity
	{
	public:
		Entity(World& world, const std::string& name);
		~Entity();
		
		virtual void Write(serialization::OutputMemoryStream& stream)
		{

		}
		virtual void Read(serialization::InputMemoryStream& stream)
		{
			
		}

		uint64_t ObjectID() const { return m_objectID; }

		template<typename Component_t, typename ...Args>
		Component_t& AddComponent(Args... args)
		{
			return m_world.Registry().emplace<Component_t>(static_cast<entt::entity>(m_entityID), std::forward<Args>(args)...);
		}

		components::Name& Name() { return m_name; }
		components::Name Name() const { return m_name; }

		components::Position& Position() { return m_pos; }
		components::Position Position() const { return m_pos; }

		components::Rotation& Rotation() { return m_rot; }
		components::Rotation Rotation() const { return m_rot; }

		components::Scale& Scale() { return m_scale; }
		components::Scale Scale() const { return m_scale; }
		

	private:
		World& m_world;
		uint32_t m_entityID;

		static uint64_t ms_lastObjectID;
		uint64_t m_objectID;

		components::Name m_name;
		components::Position m_pos;
		components::Rotation m_rot;
		components::Scale m_scale;
	}; 
}