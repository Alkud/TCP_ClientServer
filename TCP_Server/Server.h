#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#pragma comment (lib, "WS2_32.lib")

#define DEFAULT_PORT "11283"
#define MAX_CONN 0x100

class CServer
{
public:
	CServer(PCSTR portNumber = DEFAULT_PORT);		
	~CServer() {}

	int Initialize();
	int Listen();
	int Receive();

private:
	PCSTR m_PortNumber;
	PADDRINFOA m_AddrInfo;
	SOCKET m_ListenSocket;
	SOCKET m_ClientSocket;
};

