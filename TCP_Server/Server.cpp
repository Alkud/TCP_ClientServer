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

int CServer::CreateSocket()
{
	int iResult {};

	addrinfo hints{};
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(nullptr, m_PortNumber, &hints, &m_AddrInfo);
	if (0 != iResult)
		return iResult; // Resolve server address failed

	m_ListenSocket = socket(m_AddrInfo->ai_family, m_AddrInfo->ai_socktype, m_AddrInfo->ai_protocol);

	if (INVALID_SOCKET == m_ListenSocket)
		iResult = WSAGetLastError(); // Socket creation failed

	return iResult;
}
