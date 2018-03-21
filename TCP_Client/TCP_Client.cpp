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
	if (client.Initialize())
	{
		shouldExit.store(true);
	}
	while (!shouldExit.load())
	{
		if (client.run())
			shouldExit.store(true);
	}
	int i{};
}

stringVector SplitUserInput(std::string inputString)
{
	stringVector result{};
	auto first{ 0U };
	auto last{ inputString.find_first_of(' ', 0) };
	while (last != std::string::npos)
	{
		result.push_back(inputString.substr(first, last - first));
		first = last + 1;
		last = inputString.find_first_of(' ', first);
	}
	result.push_back(inputString.substr(first));
	return result;
}


int main(int argc, char* argv[])
{
	CClient client{ argv[1], argv[2] };
	std::cout << "Client created, enter an instruction or 'quit' to exit" << std::endl;
	std::cout << "client: ";
	std::string userInput{};
	stringVector command{};
	std::getline(std::cin, userInput);
	command = SplitUserInput(userInput);

	std::thread clientThread{};
	bool threadCreated{ false };

	while (true)
	{
		if ("start" == command[0])
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
				std::cout << "Client started successfully" << std::endl;
				std::cout << "client: ";
			}
		}
		else if ("send" == command[0])
		{
			client.PushTransaction(command[1]);
			std::cout << "client: ";
		}
		else if ("sendfile" == command[0])
		{
			client.PushFile(command[1]);
			std::cout << "client: ";
		}
		else if ("quit" == userInput)
		{
			shouldExit.store(true);
			break;
		}
		else
		{
			std::cout << "\nUnknown instruction";
			std::cout << "\nclient: ";
		}
		std::getline(std::cin, userInput);
		command = SplitUserInput(userInput);
	}
	if (threadCreated)
		clientThread.join();
	return 0;
}