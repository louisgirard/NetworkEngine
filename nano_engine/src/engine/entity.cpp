#include <nano_engine/components/name.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/serialization/input_memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

#include <nano_engine/replication/linking_context.hpp>

namespace nano_engine::engine
{
	Entity::Entity(engine::World& world, const std::string& name) : m_world(world), m_name(name)
	{
		m_entityID = m_world.CreateEntity();
		AddComponent<components::Name>(m_name);
	}

	Entity::~Entity()
	{
		m_world.DestroyEntity(m_entityID);
	}
}