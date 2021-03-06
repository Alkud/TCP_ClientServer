// TCP_Server.cpp in TCP_Server project

#include "stdafx.h"
#include "Server.h"
#include <iostream>
#include <string>
#include <thread>
#include <memory>

std::atomic_bool shouldExit{ false };

void process(CServer& server)
{	
	if (server.Initialize())
	{	
		shouldExit.store(true);
	}
	while (!shouldExit.load())
	{
		if (server.run())
			shouldExit.store(true);
	}
}



int main(int argc, char* argv[])
{
	CServer server{ argv[1] };	
	std::cout << "Server created, enter an instruction or 'quit' to exit" << std::endl;
	std::cout << "local server: ";
	std::string userInput{};
	std::cin >> userInput;

	std::thread serverThread{};
	bool threadCreated{ false };
	
	while (true)
	{
		if ("start" == userInput)
		{
			if (threadCreated)
			{
				std::cout << "Server is already running" << std::endl;
				std::cout << "local server: ";
			}
			else
			{
				serverThread = std::thread{ process, std::ref(server) };
				threadCreated = true;
				std::cout << "Server started successfully" << std::endl;
				std::cout << "local server: ";
			}
		}
		else if ("nclients" == userInput)
		{
			if (threadCreated)
			{
				std::cout << "Currently connected: " << server.GetNumClients() << " clients" << std::endl;
				std::cout << "local server: ";
			}
			else
			{
				std::cout << "Server not started!"<< std::endl;
				std::cout << "local server: ";
			}
		}
		else if ("list" == userInput)
		{
			if (threadCreated)
			{
				std::cout << "Received trnsactions: " << std::endl;
				server.ListTrasnsactions();
				std::cout << "local server: ";
			}
			else
			{
				std::cout << "Server not started!" << std::endl;
				std::cout << "local server: ";
			}
		}
		else if ("quit" == userInput)
		{
			shouldExit.store(true);
			break;
		}
		else
		{
			std::cout << "\nUnknown instruction";
			std::cout << "\nlocal server: ";
		}
		std::cin >> userInput;
	}
	if (threadCreated)
		serverThread.join();
    return 0;
}

