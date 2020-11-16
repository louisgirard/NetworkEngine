#include <nano_engine/replication/object_creation_registry.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/serialization/input_memory_stream.hpp>

namespace nano_engine::replication
{
	ObjectCreationRegistry& ObjectCreationRegistry::Instance()
	{
		static ObjectCreationRegistry ms_ocr;
		return ms_ocr;
	}

	engine::Entity* ObjectCreationRegistry::CreateEntity(uint32_t classID, serialization::InputMemoryStream& stream)
	{
		if (auto it = m_registry.find(classID); it != m_registry.end())
		{
			return it->second(stream);
		}

		return nullptr;
	}

	void ObjectCreationRegistry::RegisterEntityCreator(uint32_t classID, EntityCreator creator)
	{
		NANO_ASSERT(m_registry.find(classID) == m_registry.end(), "ClassID is already registered");
		NANO_ASSERT(creator != nullptr, "Creator cannot be null");

		m_registry.insert(std::make_pair(classID, creator));
	}
}