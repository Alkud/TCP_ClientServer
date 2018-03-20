#include "Client.h"



CClient::CClient(PCSTR portNumber) :
	m_ServerPortNumber{ portNumber ? portNumber : DEFAULT_PORT },
	m_ServerAddrInfo{nullptr},
	m_Socket{INVALID_SOCKET}
{
}


CClient::~CClient()
{
	WSACleanup();
}

int CClient::Initialize()
{
	WSADATA wsaDATA{};
	int iResult{};
	WORD wVersionRequested{ MAKEWORD(2, 2) };

	iResult = WSAStartup(wVersionRequested, &wsaDATA);
	return iResult;
}

int CClient::run()
{
	return 0;
}

int CClient::Connect()
{
	int iResult{};

	/* Creating a listen socket */
	addrinfo hints{};
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(nullptr, m_PortNumber, &hints, &m_AddrInfo);
	if (0 != iResult) // Resolve server address failed
	{
		// TODO log error
		return iResult;
	}

	m_ListenSocket = socket(m_AddrInfo->ai_family, m_AddrInfo->ai_socktype, m_AddrInfo->ai_protocol);
	if (INVALID_SOCKET == m_ListenSocket)  // Socket creation failed
	{
		iResult = WSAGetLastError();
		freeaddrinfo(m_AddrInfo);
		// TODO log error
		return iResult;
	}
}

int CClient::Disconnect()
{
	return 0;
}

void CClient::Send()
{
}

