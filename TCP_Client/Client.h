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

using stringVector = std::vector<std::string>;

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT "11283"

class CClient
{
public:
	CClient(PCSTR serverAddress, PCSTR serverPort);
	~CClient();

	int Initialize();	
	int run();
	
private:	
	int Connect();
	int Disconnect();
	void Send();	

	PCSTR m_ServerAddress;
	PCSTR m_ServerPortNumber;
	PADDRINFOA m_ServerAddrInfo;
	SOCKET m_Socket;
};

