#include <nano_engine/replication/replication_manager.hpp>

#include <nano_engine/components/replication_component.hpp>

#include <nano_engine/serialization/output_memory_stream.hpp>

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
	};

	void ReplicationManager::EndFrame()
	{
	};
}