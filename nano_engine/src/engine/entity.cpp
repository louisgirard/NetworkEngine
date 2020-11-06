#include <nano_engine/engine/entity.hpp>

namespace nano_engine::engine
{
	uint64_t Entity::ms_lastObjectID = 0;

	Entity::Entity(World& world, const std::string& name) : m_world(world)
	{
		m_objectID = ms_lastObjectID++;
		m_entityID = m_world.CreateEntity();

		m_name = AddComponent<components::Name>(name);
		m_pos = AddComponent<components::Position>(0.f, 0.f, 0.f);
		m_rot = AddComponent<components::Rotation>(0.f, 0.f, 0.f, 0.f);
		m_scale = AddComponent<components::Scale>(1.f, 1.f, 1.f);
	}

	Entity::~Entity()
	{

	}
}