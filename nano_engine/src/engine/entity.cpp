#include <nano_engine/components/name.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/serialization/input_memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

#include <nano_engine/replication/linking_context.hpp>

namespace nano_engine::engine
{
	Entity::Entity(std::weak_ptr<World> world, const std::string& name) : m_world(world), m_name(name)
	{
		m_objectID = replication::LinkingContext::Instance().AddEntity(this);

		m_entityID = CurrentWorld()->CreateEntity();
		AddComponent<components::Name>(m_name);
	}

	Entity::~Entity()
	{
		replication::LinkingContext::Instance().RemoveEntity(m_objectID);
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