#include "Server.h"
#include <cstring>
#include <thread>
#include <algorithm>
#include <memory>
#include <iostream>

CServer::CServer(PCSTR portNumber) :
	m_PortNumber{ portNumber ? portNumber : DEFAULT_PORT }, m_AddrInfo{ nullptr },
	m_ListenSocket { INVALID_SOCKET}
{
	m_NumClients.store(0);
}

CServer::~CServer()
{
	WSACleanup();
}

int CServer::Initialize()
{
	WSADATA wsaDATA{};
	int iResult{};
	WORD wVersionRequested{ MAKEWORD(2, 2) };

	iResult = WSAStartup(wVersionRequested, &wsaDATA);
	return iResult;
}

int CServer::run()
{
	int iResult{Listen()};
	if (iResult)
	{
		return iResult;
	}
	Receive();
	return 0;
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

	/* Bind listen socket*/
	iResult = bind(m_ListenSocket, m_AddrInfo->ai_addr, static_cast<int>(m_AddrInfo->ai_addrlen));
	if (SOCKET_ERROR == iResult) // Bind failed
	{
		freeaddrinfo(m_AddrInfo);
		closesocket(m_ListenSocket);
		// TODO log error
		return iResult;
	}

	freeaddrinfo(m_AddrInfo);

	//TODO log "listen for connections"

	/* Listen for client connections */
	iResult = listen(m_ListenSocket, MAX_CONN);
	if (SOCKET_ERROR == iResult) // Bind failed
	{	
		closesocket(m_ListenSocket);	
		// TODO log error
		return iResult;
	}	
	return iResult;
}

void CServer::Receive()
{
	int iResult{};
	sockaddr_in clientAddress{};
	int clientAddressSize{ sizeof(clientAddress) };

	std::shared_ptr<SOCKET> clientSocket{ new SOCKET{accept(m_ListenSocket,
						    reinterpret_cast<sockaddr*>(&clientAddress),
						    &clientAddressSize) } };
	while (*clientSocket != INVALID_SOCKET)
	{
		m_NumClients.store(m_NumClients.load() + 1);
		HOSTENT* clientHost;
		clientHost = gethostbyaddr(reinterpret_cast<char*>(&clientAddress.sin_addr.S_un.S_addr),
									4, AF_INET);

		// TODO log "accepted, client host name"
		std::thread newThread{ [=] {this->GetClientData(clientSocket.get()); } };
	}
}

void CServer::ListTrasnsactions()
{
	dataMutex.lock();
	for (auto& item : receivedData)
	{
		std::cout << item << std::endl;
	}
	dataMutex.unlock();
}

void CServer::GetClientData(const SOCKET* clientSocket)
{
	if (nullptr == clientSocket)
		return;

	stringVector result{};

	int bytesReceived{};
	char inputBuffer[1000];
	int iResult{ recv(*clientSocket, inputBuffer, bytesReceived, 0) };
	while (iResult != SOCKET_ERROR && bytesReceived > 0)
	{
		std::string packetString{ std::string(inputBuffer, bytesReceived) };
		if (CheckTransaction(transactionString, '|'))
		{
			dataMutex.lock();
			receivedData.push_back(transactionString);
			dataMutex.unlock();
		}
	}

	if (iResult)
	{
		//TODO log recv error
	}

	//TODO log "From client [addr]: received # transactions"

	std::string response{ "Received " + std::to_string(result.size()) + " transactions" };
	send(*clientSocket, response.data(), response.length(), 0);

	/* Disconnect the client */
	iResult = shutdown(*clientSocket, SD_SEND);
	if (SOCKET_ERROR == iResult)
	{
		// TODO log shutdown error
	}

	closesocket(*clientSocket);
}

stringVector CServer::SplitPacket(const std::string & packet, const char delimiter)
{
	stringVector result{};
	auto first{ 0U };
	auto last{ packet.find_first_of(delimiter, 0) };
	size_t delimitersCount{};
	std::string transactionString{};
	while (last != std::string::npos)
	{
		delimitersCount++;
		if (5 == delimitersCount)
		{
			transactionString += packet.substr(first, last - first + 1);
			result.push_back(transactionString);
			transactionString.clear();
		}
		else
		{
			transactionString += packet.substr(first, last - first + 1);
		}
		first = last + 1;
		last = packet.find_first_of(delimiter, first);
	}
	result.push_back(packet.substr(first));
	return result;
}

bool CServer::CheckPacket(const std::string & packet, const char delimiter)
{
	size_t first{ 0U };
	size_t last{ packet.find_first_of(delimiter, 0) };
	size_t delimitersCount{};
	while (last != std::string::npos)
	{
		delimitersCount++;
		first = last + 1;
		last = packet.find_first_of(delimiter, first);
	}
	return (delimitersCount % 5 == 0);
}

bool CServer::CheckTransaction(const std::string& transaction, const char delimiter)
{
	if (transaction.empty())
		return false;
	size_t first{ 0U };
	size_t last{ transaction.find_first_of(delimiter, 0) };
	size_t delimitersCount{};
	while (last != std::string::npos)
	{
		delimitersCount++;
		first = last + 1;
		last = transaction.find_first_of(delimiter, first);
	}
	return (delimitersCount == 5);
}

stringVector CServer::SplitTransaction(const std::string& transaction, const char delimiter)
{
	stringVector result{};
	auto first{ 0U };
	auto last{ transaction.find_first_of(delimiter, 0) };
	while (last != std::string::npos)
	{
		result.push_back(transaction.substr(first, last - first));
		first = last + 1;
		last = transaction.find_first_of(delimiter, first);
	}
	result.push_back(transaction.substr(first));
	return result;
}