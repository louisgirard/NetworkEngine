#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <iostream>
#include <string>
#include <bitset>
#include <map>
#include <time.h>
#include <thread>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#define inet_pton(af,str,addr) InetPton(af,str,addr)
#define close(s) closesocket(s)
#else
#define SOCKET int
#define INVALID_SOCKET -1
#define SD_BOTH SHUT_RDWR

#include <algorithm>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#endif

namespace portable_socket
{
#ifdef _WIN32
	struct WinInitializer
	{
		WinInitializer()
		{
			// Initialize WinSock
			WSADATA data;
			int result = WSAStartup(MAKEWORD(2, 2), &data);
			if (result != 0)
			{
				std::cout << "Error initializing WinSock" << std::endl;
			}
		}

		~WinInitializer()
		{
			// Uninitialize WinSock
			WSACleanup();
		}
	};
#endif

	enum class Protocol { UDP, TCP, ReliableUDP };

	enum class AddressFamily { IPV4, IPV6 };

	struct RecvFromResult
	{
		size_t receivedSize;
		char fromIp[INET6_ADDRSTRLEN];
		uint16_t fromPort;
	};

	struct SendToData
	{
		std::string data = "";
		clock_t timer;
		std::string toIp = "";
		uint16_t toPort;
	};

	class SpinLockGuard
	{
	public:
		explicit SpinLockGuard(std::atomic_flag& flag) : m_flag(flag)
		{
			while (m_flag.test_and_set(std::memory_order_acquire));
		}

		~SpinLockGuard()
		{
			m_flag.clear(std::memory_order_release);
		}
	private:
		std::atomic_flag& m_flag;
	};

	template<Protocol proto, AddressFamily af>
	class Socket
	{
	public:
		Socket()
		{
			if constexpr (af == AddressFamily::IPV4 && proto == Protocol::TCP)
			{
				m_socket = socket(AF_INET, SOCK_STREAM, 0);
			}
			else if constexpr (af == AddressFamily::IPV4 && proto == Protocol::UDP || proto == Protocol::ReliableUDP)
			{
				m_socket = socket(AF_INET, SOCK_DGRAM, 0);
			}
			else if constexpr (af == AddressFamily::IPV6 && proto == Protocol::TCP)
			{
				m_socket = socket(AF_INET6, SOCK_STREAM, 0);
			}
			else
			{
				m_socket = socket(AF_INET6, SOCK_DGRAM, 0);
			}

			if constexpr (proto == Protocol::ReliableUDP)
			{
				processTimersThread = std::thread(&Socket::ProcessTimers, this);
			}
		}

		explicit Socket(SOCKET s) : m_socket(s)
		{

		}

		~Socket()
		{
			Close();
		}

		void Close()
		{
			if (m_socket == INVALID_SOCKET) { return; }
			if constexpr (proto == Protocol::TCP)
			{
				shutdown(m_socket, SD_BOTH);
			}
			close(m_socket);
			m_socket = INVALID_SOCKET;

			if constexpr (proto == Protocol::ReliableUDP)
			{
				if (processTimersThread.joinable()) processTimersThread.join();
			}
		}

		auto GetLastError()
		{
#ifdef _WIN32
			return WSAGetLastError();
#else
			return strerror(errno);
#endif			
		}

		bool Bind(const std::string& iface, uint16_t port)
		{
			if (m_socket == INVALID_SOCKET) { return false; }

			auto addr = ParseEndpoint(iface, port);
			int result = bind(m_socket, (sockaddr*)&addr, sizeof(addr));
			return (result == 0);
		}

		bool Listen(uint16_t backlog)
		{
			if constexpr (proto != Protocol::TCP) { return false; }
			if (m_socket == INVALID_SOCKET) { return false; }

			int result = listen(m_socket, backlog);
			return (result == 0);
		}

		std::shared_ptr<Socket> Accept()
		{
			if constexpr (proto != Protocol::TCP) { return nullptr; }
			if (m_socket == INVALID_SOCKET) { return nullptr; }
			SOCKET client = accept(m_socket, nullptr, 0);

			if (client == INVALID_SOCKET)
			{
				return nullptr;
			}
			return std::make_shared<Socket<proto, af>>(client);
		}

		bool Connect(const std::string& server, uint16_t port)
		{
			if constexpr (proto != Protocol::TCP) { return false; }
			if (m_socket == INVALID_SOCKET) { return false; }

			auto addr = ParseEndpoint(server, port);
			int result = connect(m_socket, (sockaddr*)&addr, sizeof(addr));
			return(result == 0);
		}

		std::optional<size_t> Send(const char* data, size_t size)
		{
			if constexpr (proto != Protocol::TCP) { return {}; }
			if (m_socket == INVALID_SOCKET) { return {}; }

			int result = send(m_socket, data, size, 0);
			if (result < 0)
			{
				return {};
			}
			return result;
		}

		std::optional<size_t> Receive(char* data, size_t size)
		{
			if constexpr (proto != Protocol::TCP) { return {}; }
			if (m_socket == INVALID_SOCKET) { return {}; }

			int result = recv(m_socket, data, size, 0);
			if (result <= 0)
			{
				return {};
			}
			return result;
		}

		std::optional<size_t> SendTo(const std::string& to, uint16_t port, const char* data, size_t size)
		{
			if constexpr (proto == Protocol::TCP) { return {}; }
			if (m_socket == INVALID_SOCKET) { return {}; }

			auto addr = ParseEndpoint(to, port);
			int result;
			if constexpr (proto == Protocol::UDP)
			{
				result = sendto(m_socket, data, size, 0, (sockaddr*)&addr, sizeof(addr));
			}
			else
			{
				std::string fullData;
				uint16_t outgoingSequenceNumber = GetOutgoingSequenceNumber(to, port);
				std::string sequenceNumber = std::bitset<16>(outgoingSequenceNumber).to_string();
				SetOutgoingSequenceNumber((outgoingSequenceNumber + 1) % 65536, to, port);
				fullData = sequenceNumber + std::string(data, size);
				std::string checksum = std::bitset<32>(Checksum(fullData.c_str(), fullData.size())).to_string();
				// Reliable UDP Send
				fullData = checksum + fullData;
				result = sendto(m_socket, fullData.c_str(), fullData.size(), 0, (sockaddr*)&addr, sizeof(addr));
				// Add packet to the list of packets waiting for acknowledgment
				{
					SpinLockGuard packetsSpinLockGuard(packetsWaitingForAckSpinLock);
					if (packetsWaitingForAck.find(sequenceNumber) == packetsWaitingForAck.end())
					{
						SendToData packetData = { fullData, clock(), to, port };
						packetsWaitingForAck.insert(std::pair<std::string, SendToData>(sequenceNumber, packetData));
					}
				}

				// Remove checksum and sequence number from result size
				result -= 32 + 16;
			}

			if (result < 0)
			{
				return {};
			}
			return result;
		}

		std::optional<size_t> SendAckTo(const std::string& to, uint16_t port, uint16_t sequenceNumber)
		{
			if constexpr (proto == Protocol::TCP || proto == Protocol::UDP) { return {}; }
			if (m_socket == INVALID_SOCKET) { return {}; }

			auto addr = ParseEndpoint(to, port);

			std::string data = std::bitset<16>(sequenceNumber).to_string() + "ACK";
			std::string checksum = std::bitset<32>(Checksum(data.c_str(), data.size())).to_string();
			// Reliable UDP Send
			data = checksum + data;
			int result = sendto(m_socket, data.c_str(), data.size(), 0, (sockaddr*)&addr, sizeof(addr));

			if (result < 0)
			{
				return {};
			}
			return result;
		}

		std::optional<RecvFromResult> ReceiveFrom(char* data, size_t size)
		{
			if constexpr (proto == Protocol::TCP) { return {}; }
			if (m_socket == INVALID_SOCKET) { return {}; }

			RecvFromResult result;

			int err;
			if constexpr (af == AddressFamily::IPV4)
			{
				struct sockaddr_in addr {};
#ifdef _WIN32
				int fromlen = sizeof(addr);
#else
				unsigned int fromlen = sizeof(addr);
#endif
				memset(&addr, 0, sizeof(addr));
				addr.sin_family = AF_INET;
				err = recvfrom(m_socket, data, size, 0, (sockaddr*)&addr, &fromlen);
				inet_ntop(addr.sin_family, (void*)&addr.sin_addr, result.fromIp, INET_ADDRSTRLEN);
				result.fromPort = ntohs(addr.sin_port);
				result.receivedSize = (err >= 0) ? err : 0;
			}
			else
			{
				struct sockaddr_in6 addr {};
#ifdef _WIN32
				int fromlen = sizeof(addr);
#else
				unsigned int fromlen = sizeof(addr);
#endif
				memset(&addr, 0, sizeof(addr));
				addr.sin6_family = AF_INET6;
				err = recvfrom(m_socket, data, size, 0, (sockaddr*)&addr, &fromlen);
				inet_ntop(addr.sin6_family, (void*)&addr.sin6_addr, result.fromIp, INET6_ADDRSTRLEN);
				result.fromPort = ntohs(addr.sin6_port);
				result.receivedSize = (err >= 0) ? err : 0;
			}

			if constexpr (proto == Protocol::ReliableUDP)
			{
				// Compare the checksum
				std::string stringData(data, result.receivedSize);
				std::string oldChecksum(data, 32);
				stringData.erase(0, 32);
				result.receivedSize -= 32;
				std::string currentChecksum = std::bitset<32>(Checksum(stringData.c_str(), stringData.size())).to_string();

				// Checksum not ok, drop packet
				if (oldChecksum.compare(currentChecksum) != 0)
				{
					return {};
				}

				// Check if its an acknowledgment packet
				if (IsAckPacket(stringData))
				{
					std::string sequenceNumber = std::string(stringData.c_str(), 16);
					{
						SpinLockGuard packetsSpinLockGuard(packetsWaitingForAckSpinLock);
						packetsWaitingForAck.erase(sequenceNumber);
					}

					std::cout << "Ack received" << std::endl;
					return {};
				}

				// Check sequence number
				uint16_t sequenceNumber = std::stoi(std::string(stringData.c_str(), 16), nullptr, 2);

				uint16_t incomingSequenceNumber = GetIncomingSequenceNumber(result.fromIp, result.fromPort);

				if (MoreRecent(sequenceNumber, incomingSequenceNumber))
				{
					// Next packet in the sequence
					if (sequenceNumber == (incomingSequenceNumber + 1) % 65536)
					{
						SetIncomingSequenceNumber(sequenceNumber, result.fromIp, result.fromPort);

						stringData.erase(0, 16);
						strcpy(data, stringData.c_str());
						result.receivedSize -= 16;

						// Send acknowledgment
						SendAckTo(result.fromIp, result.fromPort, sequenceNumber);
					}
					else
					{
						// Early packet, put it in queue
						pendingPackets.insert(std::pair<std::string, RecvFromResult>(stringData, result));
						// Send acknowledgment
						SendAckTo(result.fromIp, result.fromPort, sequenceNumber);

						return {};
					}
				}
				else
				{
					// Send acknowledgment
					SendAckTo(result.fromIp, result.fromPort, sequenceNumber);
					return {};
				}
			}

			if (result.receivedSize < 0)
			{
				return {};
			}
			return result;
		}

		static int Select(std::vector<std::shared_ptr<Socket<proto, af>>>& read,
			std::vector<std::shared_ptr<Socket<proto, af>>>& write,
			std::vector<std::shared_ptr<Socket<proto, af>>>& except)
		{

			if (read.empty() && write.empty() && except.empty()) return true;

			fd_set readFDSet;
			FD_ZERO(&readFDSet);
			fd_set writeFDSet;
			FD_ZERO(&writeFDSet);
			fd_set exceptFDSet;
			FD_ZERO(&exceptFDSet);

			struct timeval timeout = { 0,100 };

			for (auto& s : read)
			{
				FD_SET(s->m_socket, &readFDSet);
			}

			for (auto& s : write)
			{
				FD_SET(s->m_socket, &writeFDSet);
			}

			for (auto& s : except)
			{
				FD_SET(s->m_socket, &exceptFDSet);
			}

			int ret = select(0, &readFDSet, &writeFDSet, &exceptFDSet, &timeout);

			// Delete socket from set
			for (int i = 0; i < read.size(); i++)
			{
				if (FD_ISSET(read[i]->m_socket, &readFDSet)) continue;
				read.erase(read.begin() + i);
				i--;
			}
			for (int i = 0; i < write.size(); i++)
			{
				if (FD_ISSET(write[i]->m_socket, &writeFDSet)) continue;
				write.erase(write.begin() + i);
				i--;
			}
			for (int i = 0; i < except.size(); i++)
			{
				if (FD_ISSET(except[i]->m_socket, &exceptFDSet)) continue;
				except.erase(except.begin() + i);
				i--;
			}

			return ret;
		}

		std::optional<RecvFromResult> NextPendingPacket(char* data)
		{
			if (pendingPackets.empty()) return {};

			// If it exists, find the packet with the next incoming sequence number
			for (auto it = pendingPackets.begin(); it != pendingPackets.end(); it++)
			{
				std::string stringData = it->first;
				uint16_t sequenceNumber = std::stoi(std::string(stringData.c_str(), 16), nullptr, 2);
				uint16_t incomingSequenceNumber = GetIncomingSequenceNumber(it->second.fromIp, it->second.fromPort);
				if (sequenceNumber == (incomingSequenceNumber + 1) % 65536)
				{
					SetIncomingSequenceNumber(sequenceNumber, it->second.fromIp, it->second.fromPort);

					stringData.erase(0, 16);
					strcpy(data, stringData.c_str());

					RecvFromResult result;
					strcpy(result.fromIp, it->second.fromIp);
					result.fromPort = it->second.fromPort;
					result.receivedSize = stringData.size();

					// Remove packet from list
					pendingPackets.erase(it);

					return result;
				}
			}

			return {};
		}

	private:
		static constexpr auto ParseEndpoint(const std::string& ipAddr, uint16_t port)
		{
			if constexpr (af == AddressFamily::IPV4)
			{
				struct sockaddr_in addr {};
				addr.sin_family = AF_INET;
				addr.sin_port = htons(port);
				inet_pton(AF_INET, ipAddr.c_str(), &addr.sin_addr);
				return addr;
			}
			else
			{
				struct sockaddr_in6 addr {};
				addr.sin6_family = AF_INET6;
				addr.sin6_port = htons(port);
				inet_pton(AF_INET6, ipAddr.c_str(), &addr.sin6_addr);
				return addr;
			}
		}

		// ---------------------- Reliable UDP ---------------------- //
		uint32_t Checksum(const char* data, size_t size) {
			uint32_t crc = 0xFFFFFFFF;

			for (size_t i = 0; i < size; i++) {
				char ch = data[i];
				for (size_t j = 0; j < 8; j++) {
					uint32_t b = (ch ^ crc) & 1;
					crc >>= 1;
					if (b) crc = crc ^ 0xEDB88320;
					ch >>= 1;
				}
			}
			return crc;
		}

		bool MoreRecent(uint16_t s1, uint16_t s2)
		{
			if (s1 > s2)
			{
				return (s1 - s2 <= 32768);
			}
			else if (s2 > s1)
			{
				return (s2 - s1 > 32768);
			}
			else
			{
				return false;
			}
		}

		void ProcessTimers()
		{
			while (m_socket != INVALID_SOCKET)
			{
				{
					SpinLockGuard packetsSpinLockGuard(packetsWaitingForAckSpinLock);
					for (auto it = packetsWaitingForAck.begin(); it != packetsWaitingForAck.end(); it++)
					{
						float time = (float)(clock() - it->second.timer) / CLOCKS_PER_SEC;
						if (time >= waitAckTime)
						{
							// Timer expired, no ack received, resend packet
							std::cout << "Timer expired for packet : " << it->first << std::endl;
							auto addr = ParseEndpoint(it->second.toIp, it->second.toPort);
							sendto(m_socket, it->second.data.c_str(), it->second.data.size(), 0, (sockaddr*)&addr, sizeof(addr));
							// Reset timer
							it->second.timer = clock();
						}
					}
				}
			}
		}

		bool IsAckPacket(const std::string& packet)
		{
			std::string data = packet;
			data.erase(0, 16);
			return (data.compare("ACK") == 0);
		}

		uint16_t GetOutgoingSequenceNumber(const std::string& ip, uint16_t port)
		{
			auto it = clientsSequenceNumbers.find(std::pair<std::string, uint16_t>(ip, port));
			if (it != clientsSequenceNumbers.end())
			{
				return it->second.second;
			}
			else
			{
				clientsSequenceNumbers.insert(std::pair<std::pair<std::string, uint16_t>, std::pair<uint16_t, uint16_t>>(std::pair<std::string, uint16_t>(ip, port), std::pair<uint16_t, uint16_t>(0xFFFF, 0)));
				return 0;
			}
		}

		void SetOutgoingSequenceNumber(uint16_t sequenceNumber, const std::string& ip, uint16_t port)
		{
			auto it = clientsSequenceNumbers.find(std::pair<std::string, uint16_t>(ip, port));
			if (it != clientsSequenceNumbers.end())
			{
				it->second.second = sequenceNumber;
			}
			else
			{
				clientsSequenceNumbers.insert(std::pair<std::pair<std::string, uint16_t>, std::pair<uint16_t, uint16_t>>(std::pair<std::string, uint16_t>(ip, port), std::pair<uint16_t, uint16_t>(0xFFFF, sequenceNumber)));
			}
		}

		uint16_t GetIncomingSequenceNumber(const std::string& ip, uint16_t port)
		{
			auto it = clientsSequenceNumbers.find(std::pair<std::string, uint16_t>(ip, port));
			if (it != clientsSequenceNumbers.end())
			{
				return it->second.first;
			}
			else
			{
				clientsSequenceNumbers.insert(std::pair<std::pair<std::string, uint16_t>, std::pair<uint16_t, uint16_t>>(std::pair<std::string, uint16_t>(ip, port), std::pair<uint16_t, uint16_t>(0xFFFF, 0)));
				return 0xFFFF;
			}
		}

		void SetIncomingSequenceNumber(uint16_t sequenceNumber, const std::string& ip, uint16_t port)
		{
			auto it = clientsSequenceNumbers.find(std::pair<std::string, uint16_t>(ip, port));
			if (it != clientsSequenceNumbers.end())
			{
				it->second.first = sequenceNumber;
			}
			else
			{
				clientsSequenceNumbers.insert(std::pair<std::pair<std::string, uint16_t>, std::pair<uint16_t, uint16_t>>(std::pair<std::string, uint16_t>(ip, port), std::pair<uint16_t, uint16_t>(sequenceNumber, 0)));
			}
		}

		// Preserve Order
		std::map<std::string, RecvFromResult> pendingPackets; // packets that arrived early -> (size, ip, port)
		std::map<std::pair<std::string, uint16_t>, std::pair<uint16_t, uint16_t>> clientsSequenceNumbers; // Save client IP and port associated with their current sequence numbers, to know if it's a new client

		// Reliability 
		std::map<std::string, SendToData> packetsWaitingForAck; // sequence number -> (packet, timer, ip, port)
		std::atomic_flag packetsWaitingForAckSpinLock = ATOMIC_FLAG_INIT;
		std::thread processTimersThread; // Process Timers, to see if a packet needs to be sent again
		float waitAckTime = 2;

		// ---------------------- End Reliable UDP ---------------------- //

		SOCKET m_socket = INVALID_SOCKET;

#ifdef _WIN32
		static WinInitializer ms_initializer;
#endif
	};
#ifdef _WIN32
	WinInitializer Socket<Protocol::TCP, AddressFamily::IPV4>::ms_initializer;
	WinInitializer Socket<Protocol::TCP, AddressFamily::IPV6>::ms_initializer;
	WinInitializer Socket<Protocol::UDP, AddressFamily::IPV4>::ms_initializer;
	WinInitializer Socket<Protocol::UDP, AddressFamily::IPV6>::ms_initializer;
	WinInitializer Socket<Protocol::ReliableUDP, AddressFamily::IPV4>::ms_initializer;
	WinInitializer Socket<Protocol::ReliableUDP, AddressFamily::IPV6>::ms_initializer;
#endif
}