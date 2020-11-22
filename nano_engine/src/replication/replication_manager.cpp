#include <nano_engine/replication/replication_manager.hpp>

#include <nano_engine/components/replication_component.hpp>

#include <nano_engine/serialization/output_memory_stream.hpp>
#include <nano_engine/serialization/input_memory_stream.hpp>

#include <nano_engine/replication/linking_context.hpp>
#include <nano_engine/replication/object_creation_registry.hpp>

#include <portable_socket/portable_socket.hpp>

namespace nano_engine::replication
{
	class ReplicationManagerImpl
	{
		using ReplicationSocket = portable_socket::Socket<portable_socket::Protocol::UDP, portable_socket::AddressFamily::IPV4>;
		static constexpr size_t RXBufferSize = 8 * 1024;

		enum PacketType : uint8_t
		{
			PKT_HELLO,
			PKT_REPLICATION_DATA,
			PKY_BYE,
			PKT_COUNT
		};
	public:
		class ReplicationManagerImpl(bool server, const std::string& ip, uint16_t port)
		{
			m_server = server;
			m_socket = std::make_shared<ReplicationSocket>();

			m_serverIp = ip;
			m_serverPort = port;

			if (m_server)
			{
				m_socket->Bind(ip, port);
				return;
			}

			//Create connection packet
			char packet[1];
			packet[0] = PacketType::PKT_HELLO;
			m_socket->SendTo(m_serverIp, m_serverPort, nullptr, 0);
		}

		void BeginFrame()
		{

		}

		void Update(std::chrono::microseconds deltaTime, engine::World& world)
		{
			m_read.push_back(m_socket);

			portable_socket::RecvFromResult socketResult{};

			if (ReplicationSocket::Select(m_read, m_write, m_except) > 0)
			{
				auto result = m_socket->ReceiveFrom(m_rxBuffer, RXBufferSize);
				if (!result && !m_server) return;
				socketResult = result.value();
			}


			if (!m_server)
			{
				//Read socket
				//Interpret replication packet

			}
			else
			{
				UpdateServer(socketResult, world);
			}
			
		}

		void EndFrame()
		{

		}

	private:
		LinkingContext m_linkingContext;
		std::shared_ptr<ReplicationSocket> m_socket;
		std::vector<std::shared_ptr<ReplicationSocket>> m_read;
		std::vector<std::shared_ptr<ReplicationSocket>> m_write;
		std::vector<std::shared_ptr<ReplicationSocket>> m_except;

		bool m_isConnected = false;
	
		bool m_server;
		std::string m_serverIp;
		uint16_t m_serverPort;

		std::string m_clientIp;
		uint16_t m_clientPort;

		char m_rxBuffer[RXBufferSize];

		void UpdateClient(const portable_socket::RecvFromResult& sockResult, engine::World& world)
		{

		}

		void UpdateServer(const portable_socket::RecvFromResult& sockResult, engine::World& world)
		{
			//Read incoming data
			if (sockResult.receivedSize > 0)
			{
				//If connected and data received check if disconnection
				if (m_isConnected)
				{
					m_isConnected = (m_rxBuffer[0] == PacketType::PKY_BYE);
				}
				else
				{
					//Try to connect
					m_isConnected = (m_rxBuffer[0] == PacketType::PKT_HELLO);
					m_clientIp = sockResult.fromIp;
					m_clientPort = sockResult.fromPort;
				}
			}

			if (!m_isConnected) return;

			//Create our replication packet
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

			//Send data to network
			m_socket->SendTo(m_clientIp, m_clientPort, stream.Data(), stream.Size());
		}
	};

	ReplicationManager::ReplicationManager(bool server, const std::string& ip, uint16_t port)
	{
		m_impl = std::make_shared<ReplicationManagerImpl>(server, ip, port);
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