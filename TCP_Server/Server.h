#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <atomic>
#include <mutex>

#pragma comment (lib, "WS2_32.lib")

#define DEFAULT_PORT "11283"
#define MAX_CONN 0x100

using stringVector = std::vector<std::string>;

class CServer
{
public:
	CServer(PCSTR portNumber);		
	~CServer();

	int Initialize();
	
	int run();

	int GetNumClients()
	{ return m_NumClients.load();}

	void ListTrasnsactions();

private:
	int Listen();
	void Receive();
	void GetClientData(const SOCKET* clientSocket);
	bool CheckTransaction(const std::string& transaction, const char delimiter);
	stringVector SplitTransaction(const std::string& transaction, const char delimiter);

	PCSTR m_PortNumber;
	PADDRINFOA m_AddrInfo;
	SOCKET m_ListenSocket;

	std::atomic_int m_NumClients;

	std::mutex dataMutex;
	stringVector receivedData;
};

