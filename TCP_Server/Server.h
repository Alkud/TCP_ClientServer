#pragma once

#include <windows.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#pragma comment (lib, "WS2_32.lib")

#define DEFAULT_PORT "11283"

class CServer
{
public:
	CServer(PCSTR portNumber = DEFAULT_PORT);		
	~CServer() {}

	int Initialize();
	int CreateSocket();

private:
	PCSTR m_PortNumber;
	PADDRINFOA m_AddrInfo;
	SOCKET m_ListenSocket;
	SOCKET m_ClientSocket;
};

