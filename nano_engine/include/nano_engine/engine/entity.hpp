#pragma once

#include <type_traits>

#include <nano_engine/common.hpp>

#include <nano_engine/engine/world.hpp>

#include <nano_engine/replication/object_creation_registry.hpp>
#include <nano_engine/replication/linking_context.hpp>

#include <nano_engine/components/name.hpp>
#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rotation.hpp>
#include <nano_engine/components/scale.hpp>

#include <nano_engine/components/rigid_body.hpp>


#define REPLICATED(id, className) \
enum { classID = id }; \
virtual uint32_t GetClassID() { return classID; } \
static Entity* CreateEntity(serialization::InputMemoryStream& stream); \
struct __Registrator \
{ \
	__Registrator() \
	{ \
		replication::ObjectCreationRegistry::Instance().RegisterEntityCreator(classID, &className::CreateEntity); \
	} \
}; \
static __Registrator ms___registrator; \

namespace nano_engine::engine
{
	using ObjectID_t = uint64_t;
	class Entity
	{
	public:
		REPLICATED('ENTI', Entity);

		Entity(engine::World& world, const std::string& name);
		virtual ~Entity();

		virtual void Write(serialization::OutputMemoryStream& stream) const;
		virtual void Read(serialization::InputMemoryStream& stream);

		const components::Position& GetPosition() const { return m_position; }
		virtual void SetPosition(float x, float y, float z);

		components::Rotation& GetRotation() { return m_rotation; }
		const components::Rotation& GetRotation() const { return m_rotation; }

		components::Scale& GetScale() { return m_scale; }
		const components::Scale& GetScale() const { return m_scale; }

		const components::Name& GetName() const { return m_name; }

		virtual components::RigidBody* GetRigidBody() { return nullptr; }

		bool Replicate() { return m_replicate; }
	protected:
		bool m_replicate = false;

	private:
		World& m_world;

		components::Name m_name;
		components::Position m_position;
		components::Rotation m_rotation;
		components::Scale m_scale;
	};
}