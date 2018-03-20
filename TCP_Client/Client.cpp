#include "Client.h"



CClient::CClient(PCSTR serverAddress, PCSTR serverPort) :
	m_ServerAddress{ serverAddress ? serverAddress : DEFAULT_ADDRESS },
	m_ServerPortNumber{ serverPort ? serverPort : DEFAULT_PORT},
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
	hints.ai_flags = AI_NUMERICHOST;

	

	iResult = getaddrinfo(m_ServerAddress, m_ServerPortNumber, &hints, &m_ServerAddrInfo);
	if (0 != iResult) // Resolve server address failed
	{
		// TODO log error
		return iResult;
	}

	

	/* Attempt to conect to an address until one succeeds */
	for (PADDRINFOA tempAddrInfo{ m_ServerAddrInfo }; tempAddrInfo != nullptr;
		tempAddrInfo = tempAddrInfo->ai_next)
	{
		m_Socket = socket(tempAddrInfo->ai_family, tempAddrInfo->ai_socktype, tempAddrInfo->ai_protocol);
	}
	
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

