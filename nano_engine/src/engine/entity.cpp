#include <nano_engine/components/name.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/serialization/input_memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

#include <nano_engine/replication/linking_context.hpp>

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rotation.hpp>
#include <nano_engine/components/scale.hpp>

namespace nano_engine::engine
{
	Entity::Entity(engine::World& world, const std::string& name) : m_world(world)
	{
		m_name.name = name;
		m_world.CreateEntity(this);
	}

	Entity::~Entity()
	{
		m_world.DestroyEntity(this);
	}

	void Entity::SetPosition(float x, float y, float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

	void Entity::Write(serialization::OutputMemoryStream& stream) const
	{
		m_name.Write(stream);
		m_position.Write(stream);
		m_rotation.Write(stream);
		m_scale.Write(stream);
	}

	void Entity::Read(serialization::InputMemoryStream& stream)
	{
		m_name.Read(stream);
		m_position.Read(stream);
		SetPosition(m_position.x, m_position.y, m_position.z);

		m_rotation.Read(stream);
		m_scale.Read(stream);
	}
}