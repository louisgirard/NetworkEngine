#include <nano_engine/replication/replication_manager.hpp>

#include <nano_engine/components/replication_component.hpp>

#include <nano_engine/serialization/output_memory_stream.hpp>
#include <nano_engine/serialization/input_memory_stream.hpp>

#include <nano_engine/replication/object_creation_registry.hpp>

namespace nano_engine::replication
{
	enum PacketType : uint8_t
	{
		PKT_HELLO,
		PKT_REPLICATION_DATA,
		PKY_BYE,
		PKT_COUNT
	};

	void ReplicationManager::BeginFrame()
	{
	};

	void ReplicationManager::Update(std::chrono::microseconds deltaTime, engine::World& world)
	{
		serialization::OutputMemoryStream stream;
		stream.Write(PacketType::PKT_REPLICATION_DATA);

		world.Registry().view<components::ReplicationComponent>().each([&](components::ReplicationComponent& replication)
			{
				stream.Write(replication.Entity()->ObjectID());
				stream.Write(replication.Entity()->GetClassID());
				replication.Entity()->Write(stream);
			});

		//TODO : send data to network

		//////////////////////////////////

		serialization::InputMemoryStream inputStream(std::move(stream));
		NANO_ASSERT(inputStream.Read<PacketType>() == PacketType::PKT_REPLICATION_DATA, "Packet is not replication data");

		while (inputStream.Size() > 0)
		{
			auto objectID = inputStream.Read<engine::ObjectID_t>();
			auto classID = inputStream.Read<uint32_t>();
			auto entity = LinkingContext::Instance().GetEntity(objectID);
			if (entity == nullptr)
			{
				//Create entity
				entity = ObjectCreationRegistry::Instance().CreateEntity(classID, inputStream);
				NANO_ASSERT(entity != nullptr, "Entity is not registered in the ObjectRegistry");
			}
			entity->Read(inputStream);
		}
	};

	void ReplicationManager::EndFrame()
	{
	};
}