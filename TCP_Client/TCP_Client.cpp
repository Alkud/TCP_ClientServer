// TCP_ClientServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include <iostream>
#include <string>
#include <thread>
#include <memory>

std::atomic_bool shouldExit{ false };

void process(CClient& client)
{
	if (!client.Initialize())
	{
		shouldExit.store(true);
	}
	while (!shouldExit.load())
	{
		if (client.run())
			shouldExit.store(true);
	}
}

int main(int argc, char* argv[])
{
	CClient client{ argv[0], argv[1] };
	std::cout << "Client created, enter an instruction or 'quit' to exit" << std::endl;
	std::cout << "client: ";
	std::string userInput{};
	std::cin >> userInput;

	std::thread clientThread{};
	bool threadCreated{ false };

	while (true)
	{
		if ("start" == userInput)
		{
			if (threadCreated)
			{
				std::cout << "Client is already running" << std::endl;
				std::cout << "client: ";
			}
			else
			{
				clientThread = std::thread{ process, std::ref(client) };
				threadCreated = true;
				std::cout << "Server started successfully" << std::endl;
				std::cout << "local server: ";
			}
		}
		else if ("nclients" == userInput)
		{
			std::cout << "Currently connected: " << server.GetNumClients() << " clients" << std::endl;
			std::cout << "local server: ";
		}
		else if ("list" == userInput)
		{
			std::cout << "Received trnsactions: " << std::endl;
			server.ListTrasnsactions();
			std::cout << "local server: ";
		}
		else if ("quit" == userInput)
		{
			shouldExit.store(true);
			break;
		}
		std::cin >> userInput;
	}
	serverThread.join();
	return 0;
}*/