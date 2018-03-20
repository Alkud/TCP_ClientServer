#include "Server.h"
#include <cstring>



CServer::CServer(PCSTR portNumber) :
	m_PortNumber{ portNumber }, m_AddrInfo{nullptr},
	m_ListenSocket { INVALID_SOCKET}, m_ClientSocket{ INVALID_SOCKET}
{
}

int CServer::Initialize()
{
	WSADATA wsaDATA{};
	int iResult{};
	WORD wVersionRequested{ MAKEWORD(2, 2) };

	iResult = WSAStartup(wVersionRequested, &wsaDATA);
	return iResult;
}

int CServer::Listen()
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
		WSACleanup();
		return iResult; 
	}		

	m_ListenSocket = socket(m_AddrInfo->ai_family, m_AddrInfo->ai_socktype, m_AddrInfo->ai_protocol);
	if (INVALID_SOCKET == m_ListenSocket)  // Socket creation failed
	{
		iResult = WSAGetLastError();
		freeaddrinfo(m_AddrInfo);
		WSACleanup();
		return iResult;
	}	

	/* Bind listen socket*/
	iResult = bind(m_ListenSocket, m_AddrInfo->ai_addr, static_cast<int>(m_AddrInfo->ai_addrlen));
	if (SOCKET_ERROR == iResult) // Bind failed
	{
		freeaddrinfo(m_AddrInfo);
		closesocket(m_ListenSocket);
		WSACleanup();
		return iResult;
	}

	freeaddrinfo(m_AddrInfo);

	/* Listen for client connections */
	iResult = listen(m_ListenSocket, MAX_CONN);
	if (SOCKET_ERROR == iResult) // Bind failed
	{	
		closesocket(m_ListenSocket);
		WSACleanup();
		return iResult;
	}	

	return iResult;
}

int CServer::Receive()
{
	sockaddr_in clientAddress{};
	int clientAddressSize{ sizeof(clientAddress) };

	m_ClientSocket = accept(m_ListenSocket,
		static_cast<sockaddr*>(&clientAddress),
		&clientAddressSize);

	return 0;
}

