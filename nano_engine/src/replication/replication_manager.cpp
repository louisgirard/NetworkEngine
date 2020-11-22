#include <nano_engine/replication/replication_manager.hpp>

#include <nano_engine/components/replication_component.hpp>

#include <nano_engine/serialization/output_memory_stream.hpp>
#include <nano_engine/serialization/input_memory_stream.hpp>

#include <nano_engine/replication/linking_context.hpp>
#include <nano_engine/replication/object_creation_registry.hpp>

namespace nano_engine::replication
{

	class ReplicationManagerImpl
	{
		enum PacketType : uint8_t
		{
			PKT_HELLO,
			PKT_REPLICATION_DATA,
			PKY_BYE,
			PKT_COUNT
		};
	public:
		void BeginFrame()
		{

		}

		void Update(std::chrono::microseconds deltaTime, engine::World& world)
		{
			serialization::OutputMemoryStream stream;
			stream.Write(PacketType::PKT_REPLICATION_DATA);

			world.Registry().view<components::ReplicationComponent>().each([&](components::ReplicationComponent& replication)
				{
					auto netID = m_linkingContext.GetObjectID(replication.Entity());
					if (netID == 0)
					{
						netID = m_linkingContext.AddEntity(replication.Entity());
					}

					stream.Write(netID);
					stream.Write(replication.Entity()->GetClassID());
					replication.Entity()->Write(stream);
				});

			//TODO : send data to network
		}

		void EndFrame()
		{

		}

	private:
		LinkingContext m_linkingContext;
	};

	ReplicationManager::ReplicationManager()
	{
		m_impl = std::make_shared<ReplicationManagerImpl>();
	}

	ReplicationManager::~ReplicationManager()
	{
		m_impl = nullptr;
	}

	void ReplicationManager::BeginFrame()
	{
		m_impl->BeginFrame();
	};

	void ReplicationManager::Update(std::chrono::microseconds deltaTime, engine::World& world)
	{
		m_impl->Update(deltaTime, world);
	};

	void ReplicationManager::EndFrame()
	{
		m_impl->EndFrame();
	};
}