#pragma once

#include <memory>
#include <optional>
#include <vector>

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

	enum class Protocol { UDP, TCP };
	enum class AddressFamily { IPV4, IPV6 };

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
			else if constexpr (af == AddressFamily::IPV4 && proto == Protocol::UDP)
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
			if constexpr (proto == Protocol::UDP) { return false; }
			if (m_socket == INVALID_SOCKET) { return false; }

			int result = listen(m_socket, backlog);
			return (result == 0);
		}

		std::shared_ptr<Socket> Accept()
		{
			if constexpr (proto == Protocol::UDP) { return nullptr; }
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
			if constexpr (proto == Protocol::UDP) { return false; }
			if (m_socket == INVALID_SOCKET) { return false; }

			auto addr = ParseEndpoint(server, port);
			int result = connect(m_socket, (sockaddr*)&addr, sizeof(addr));
			return(result == 0);
		}

		std::optional<size_t> Send(const char* data, size_t size)
		{
			if constexpr (proto == Protocol::UDP) { return {}; }
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
			if constexpr (proto == Protocol::UDP) { return {}; }
			if (m_socket == INVALID_SOCKET) { return {}; }

			int result = recv(m_socket, data, size, 0);
			if (result <= 0)
			{
				return {};
			}
			return result;
		}

		std::optional<size_t> SendTo(const std::string& to, uint16_t port, char* data, size_t size)
		{
			if constexpr (proto == Protocol::TCP) { return {}; }
			if (m_socket == INVALID_SOCKET) { return {}; }

			auto addr = ParseEndpoint(to, port);
			int result = sendto(m_socket, data, size, 0, (sockaddr*)&addr, sizeof(addr));
			if (result < 0)
			{
				return {};
			}
			return result;
		}

		std::optional<size_t> ReceiveFrom(char* data, size_t size, sockaddr* from, int* fromlen)
		{
			if constexpr (proto == Protocol::TCP) { return {}; }
			if (m_socket == INVALID_SOCKET) { return {}; }

			int result = recvfrom(m_socket, data, size, 0, from, fromlen);

			if (result < 0)
			{
				return {};
			}
			return result;
		}

		static bool Select(std::vector<std::shared_ptr<Socket<proto, af>>>& read,
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

			struct timeval timeout = { 1,0 };

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

			return ret >= 0;
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
#endif
}