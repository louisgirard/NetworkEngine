#include <atomic>
#include <chrono>
using namespace std::literals::chrono_literals;
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <portable_socket/portable_socket.hpp>
using namespace portable_socket;

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

std::atomic_bool running = true;
std::atomic_flag clientsSpinLock = ATOMIC_FLAG_INIT;

std::vector<std::shared_ptr<Socket<Protocol::TCP, AddressFamily::IPV4>>> mg_clients;

template<Protocol proto, AddressFamily af>
void ReceiveRoutine()
{
	while (running)
	{
		std::vector<std::shared_ptr<Socket<proto, af>>> mg_read;
		std::vector<std::shared_ptr<Socket<proto, af>>> mg_write;
		std::vector<std::shared_ptr<Socket<proto, af>>> mg_except;

		{
			SpinLockGuard clientsSpinLockGuard(clientsSpinLock);
			for (auto& client : mg_clients)
			{
				mg_read.push_back(client);
			}
		}

		if (!Socket<proto, af>::Select(mg_read, mg_write, mg_except)) return;

		for (auto& client : mg_read)
		{
			// Receive
			char buffer[1024];
			auto received = client->Receive(buffer, 1024);
			if (!received)
			{
				std::cout << "Receive error : " << client->GetLastError() << std::endl;
				// Error with this client, continue with other clients
				client->Close();
				{
					SpinLockGuard clientsSpinLockGuard(clientsSpinLock);
					mg_clients.erase(std::remove(mg_clients.begin(), mg_clients.end(), client), mg_clients.end());
				}
				continue;
			}
			std::cout << "Client sent " << *received << " bytes : " << std::string(buffer, *received) << std::endl;

			std::this_thread::sleep_for(1s);

			// Send
			auto sent = client->Send(buffer, *received);
			if (!sent)
			{
				std::cout << "Send error : " << client->GetLastError() << std::endl;
				// Error with this client, continue with other clients
				client->Close();
				{
					SpinLockGuard clientsSpinLockGuard(clientsSpinLock);
					mg_clients.erase(std::remove(mg_clients.begin(), mg_clients.end(), client), mg_clients.end());
				}
				continue;
			}
			std::cout << "Server sent " << *sent << " bytes" << std::endl;
		}
	}
}

template<Protocol proto, AddressFamily af>
void ServerRoutine(Socket<proto, af>& server)
{
	while (running)
	{
		if (auto client = server.Accept(); client != nullptr)
		{
			{
				SpinLockGuard clientsSpinLockGuard(clientsSpinLock);
				mg_clients.push_back(client);
			}
		}
		else
		{
			std::cout << "Accept error : " << server.GetLastError() << std::endl;
			for (auto& client : mg_clients)
			{
				client->Close();
			}
			return;
		}
	}

	for (auto& client : mg_clients)
	{
		client->Close();
	}
}

int main(int argc, char* argv[])
{
	// Create the socket
	Socket<Protocol::TCP, AddressFamily::IPV4> server;

	// Bind the socket
	if (!server.Bind("127.0.0.1", 8888))
	{
		std::cout << "Bind error : " << server.GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	// Listen incoming connections
	if (!server.Listen(255))
	{
		std::cout << "Listen error : " << server.GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	// Accept connection
	std::thread serverThread(ServerRoutine<Protocol::TCP, AddressFamily::IPV4>, std::ref(server));
	std::thread receiveThread(ReceiveRoutine<Protocol::TCP, AddressFamily::IPV4>);

	std::cin.ignore();

	running = false;
	server.Close();

	if (serverThread.joinable()) serverThread.join();
	if (receiveThread.joinable()) receiveThread.join();

	std::cin.ignore();
}