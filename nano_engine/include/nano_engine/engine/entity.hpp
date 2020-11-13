#pragma once

#include <type_traits>

#include <nano_engine/common.hpp>

#include <nano_engine/engine/world.hpp>

namespace nano_engine::serialization
{
	class OutputMemoryStream;
	class InputMemoryStream;
}

namespace nano_engine::engine
{
	using ObjectID_t = uint64_t;
	class Entity
	{
	public:
		Entity(std::weak_ptr<World> world, const std::string& name);
		virtual ~Entity();

		virtual void Write(serialization::OutputMemoryStream& stream) const;
		virtual void Read(serialization::InputMemoryStream& stream);

		ObjectID_t ObjectID() const { return m_objectID;  }
		EntityID_t EntityID() const { return m_entityID; }

		template<typename Component_t, typename... Args>
		Component_t& AddComponent(Args... args)
		{
			return CurrentWorld()->Registry().emplace<Component_t>(static_cast<entt::entity>(m_entityID), std::forward<Args>(args)...);
		}

	protected:
		std::shared_ptr<World> CurrentWorld()
		{
			NANO_ASSERT(!m_world.expired(), "The world is not valid anymore.");
			return m_world.lock();
		}

	private:
		std::weak_ptr<World> m_world;

		EntityID_t m_entityID;

		ObjectID_t m_objectID;

		std::string m_name;
	};
}