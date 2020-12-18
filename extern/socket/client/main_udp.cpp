#include <atomic>
#include <chrono>
using namespace std::literals::chrono_literals;
#include <iostream>

#include <portable_socket/portable_socket.hpp>
using namespace portable_socket;

std::atomic_bool running = true;

Socket<Protocol::ReliableUDP, AddressFamily::IPV4> client;

void ReceiveRoutine()
{
	char buffer[1024];
	while (running)
	{
		// Receive packets
		auto received = client.ReceiveFrom(buffer, 1024);
		if (received)
		{
			std::cout << "Server sent " << received.value().receivedSize << " bytes : " << std::string(buffer, received.value().receivedSize) << std::endl;
		}

		// Process the possible pending packets / the early packets
		while (auto packet = client.NextPendingPacket(buffer))
		{
			std::cout << "Server pending packet " << packet.value().receivedSize << " bytes : " << std::string(buffer, packet.value().receivedSize) << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	std::thread receiveThread(ReceiveRoutine);

	// Send
	std::string hello = "Hello world";
	auto sent = client.SendTo("127.0.0.1", 8888, hello.c_str(), hello.size());
	if (!sent)
	{
		std::cout << "Send error : " << client.GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Client sent " << *sent << " bytes" << std::endl;

	std::cin.ignore();

	running = false;
	client.Close();

	if (receiveThread.joinable()) receiveThread.join();

	std::cin.ignore();

}