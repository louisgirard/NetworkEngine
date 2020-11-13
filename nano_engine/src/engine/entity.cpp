#include <nano_engine/components/name.hpp>

#include <nano_engine/engine/entity.hpp>

namespace nano_engine::engine
{
	uint64_t Entity::ms_lastObjectID = 0;

	Entity::Entity(std::weak_ptr<World> world, const std::string& name) : m_world(world), m_name(name)
	{
		m_objectID = ms_lastObjectID++;

		m_entityID = CurrentWorld()->CreateEntity();
		AddComponent<components::Name>(m_name);
	}

	Entity::~Entity()
	{
		CurrentWorld()->DestroyEntity(m_entityID);
	}

	void Entity::Write(serialization::OutputMemoryStream& stream) const
	{
		stream.Write(m_objectID);
	}

	void Entity::Read(serialization::InputMemoryStream& stream)
	{
		assert(stream.Read<ObjectID_t>() == m_objectID);
	}
}