#pragma once

#include <windows.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#pragma comment (lib, "WS2_32.lib")

#define DEFAULT_PORT 11283

class CServer
{
public:
	CServer(int portNumber = DEFAULT_PORT) :
		m_PortNumber{ portNumber } {}
	~CServer() {}
	int Initialize();
	int CreateSocket();

private:
	int m_PortNumber;
};

