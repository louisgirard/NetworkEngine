#pragma once

#include <functional>
#include <map>

#include <nano_engine/common.hpp>

namespace nano_engine::engine
{
	class Entity;
}

namespace nano_engine::serialization
{
	class InputMemoryStream;
}

namespace nano_engine::replication
{
	using EntityCreator = std::function<engine::Entity* (serialization::InputMemoryStream&)>;

	class ObjectCreationRegistry
	{
	public:
		static ObjectCreationRegistry& Instance();
		engine::Entity* CreateEntity(uint32_t classID, serialization::InputMemoryStream& stream);
		void RegisterEntityCreator(uint32_t classID, EntityCreator creator);

	private:
		ObjectCreationRegistry() {}
		std::map<uint32_t, EntityCreator> m_registry;
	};
}