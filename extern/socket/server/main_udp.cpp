#include <atomic>
#include <chrono>
using namespace std::literals::chrono_literals;
#include <iostream>

#include <portable_socket/portable_socket.hpp>
using namespace portable_socket;

std::atomic_bool running = true;
Socket<Protocol::ReliableUDP, AddressFamily::IPV4> server;

void ReceiveRoutine()
{
	char buffer[1024];
	while (running)
	{
		// Receive packets
		auto received = server.ReceiveFrom(buffer, 1024);
		if (received)
		{
			std::cout << "Client sent " << received.value().receivedSize << " bytes : " << std::string(buffer, received.value().receivedSize) << std::endl;

			// Send
			std::string hello = "Hello world";
			auto sent = server.SendTo(received.value().fromIp, received.value().fromPort, hello.c_str(), hello.size());
			if (sent)
			{
				std::cout << "Server sent " << *sent << " bytes" << std::endl;
			}
		}

		// Process the possible pending packets / the early packets
		while (auto packet = server.NextPendingPacket(buffer))
		{
			std::cout << "Client pending packet " << packet.value().receivedSize << " bytes : " << std::string(buffer, packet.value().receivedSize) << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	// Bind the socket
	if (!server.Bind("127.0.0.1", 8888))
	{
		std::cout << "Bind error : " << server.GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	std::thread receiveThread(ReceiveRoutine);

	std::cin.ignore();

	running = false;
	server.Close();

	if (receiveThread.joinable()) receiveThread.join();

	std::cin.ignore();
}