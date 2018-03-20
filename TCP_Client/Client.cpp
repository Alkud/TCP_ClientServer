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

	/* Creating socket */
	m_Socket = socket(m_ServerAddrInfo->ai_family, m_ServerAddrInfo->ai_socktype, m_ServerAddrInfo->ai_protocol);
	
	if (INVALID_SOCKET == m_Socket)  // Socket creation failed
	{
		iResult = WSAGetLastError();
		freeaddrinfo(m_ServerAddrInfo);
		// TODO log error
		return iResult;
	}

	/* Connect to the server */
	iResult = connect(m_Socket, m_ServerAddrInfo->ai_addr, static_cast<int>(m_ServerAddrInfo->ai_addrlen));
	if (SOCKET_ERROR == iResult)
	{
		freeaddrinfo(m_ServerAddrInfo);
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
	while (!dataToSend.empty())
	{
		int iResult{};
		size_t packetsToSend{ dataToSend.size() % 5 == 0 ? 5 : dataToSend.size() % 5 }; // Send 5 transactions or less


		iResult = send(m_Socket, dataToSend.front().data(),
			dataToSend.front().length(), 0);
	}
}

