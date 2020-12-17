#pragma once

#include <type_traits>

#include <nano_engine/common.hpp>

#include <nano_engine/engine/world.hpp>

#include <nano_engine/replication/object_creation_registry.hpp>
#include <nano_engine/replication/linking_context.hpp>

namespace nano_engine::serialization
{
	class OutputMemoryStream;
	class InputMemoryStream;
}

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

		virtual void Write(serialization::OutputMemoryStream& stream) const {}
		virtual void Read(serialization::InputMemoryStream& stream) {}

		EntityID_t EntityID() const { return m_entityID; }

		template<typename Component_t, typename... Args>
		Component_t& AddComponent(Args... args)
		{
			entt::entity id = static_cast<entt::entity>(m_entityID);
			return m_world.Registry().emplace<Component_t>(static_cast<entt::entity>(m_entityID), std::forward<Args>(args)...);
		}

	private:
		World& m_world;

		EntityID_t m_entityID;

		std::string m_name;
	};
}