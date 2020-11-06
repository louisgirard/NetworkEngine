#include <nano_engine/engine/entity.hpp>

namespace nano_engine::engine
{
	uint64_t Entity::ms_lastObjectID = 0;

	Entity::Entity(World& world, const std::string& name) : m_world(world)
	{
		m_objectID = ms_lastObjectID++;
		m_entityID = world.CreateEntity();

		m_name = AddComponent<components::Name>(name);

		m_state = std::make_shared<EntityState>(
			AddComponent<components::Position>(0.f, 0.f, 0.f),
			AddComponent<components::Rotation>(0.f, 0.f, 0.f, 0.f),
			AddComponent<components::Scale>(1.f, 1.f, 1.f)
			);
	}

	Entity::~Entity()
	{

	}

	void Entity::Write(serialization::OutputMemoryStream& stream)
	{
		m_state->m_pos.Write(stream);
		m_state->m_rot.Write(stream);
		m_state->m_scale.Write(stream);
	}

	void Entity::Read(serialization::InputMemoryStream& stream)
	{
		m_state->m_pos.Read(stream);
		m_state->m_rot.Read(stream);
		m_state->m_scale.Read(stream);
	}
}