#include <chrono>
using namespace std::literals::chrono_literals;
#include <iostream>
#include <thread>

#include <portable_socket/portable_socket.hpp>
using namespace portable_socket;

int main(int argc, char* argv[])
{
	// Create the socket
	Socket<Protocol::TCP, AddressFamily::IPV4> client;

	// Connect to the server
	if (!client.Connect("127.0.0.1", 8888))
	{
		std::cout << "Connect error : " << client.GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	// Send
	std::string hello = "Hello world";
	auto sent = client.Send(hello.c_str(), hello.size());
	if (!sent)
	{
		std::cout << "Send error : " << client.GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Client sent " << *sent << " bytes" << std::endl;

	// Receive
	char buffer[1024];
	auto received = client.Receive(buffer, 1024);
	if (!received)
	{
		std::cout << "Receive error : " << client.GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Server sent " << *received << " bytes : " << std::string(buffer, *received) << std::endl;

	std::cin.ignore();
}