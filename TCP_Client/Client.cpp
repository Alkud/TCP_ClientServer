#include "Client.h"
#include <fstream>
#include <string>
#include <iostream>


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
	if (Connect())
		Send();
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
	/* Disconnect the client */
	int iResult{ shutdown(m_Socket, SD_SEND) };
	if (SOCKET_ERROR == iResult)
	{
		// TODO log shutdown error
	}
	closesocket(m_Socket);
}

void CClient::Send()
{
	int iResult{};
	/* Send queued transactions */
	while (!dataToSend.empty())
	{	
		size_t packetsToSend{ dataToSend.size() >= 5 ? 5 : dataToSend.size() % 5 }; // Send 5 transactions or less

		iResult = send(m_Socket, dataToSend.front().data(),
			dataToSend.front().length(), 0);
		if (SOCKET_ERROR == iResult)
		{
			// TODO log send error
			closesocket(m_Socket);
			break;
		}
	}
	/* Receive server reply */
	int bytesReceived{};
	char inputBuffer[1000];
	do
	{
		iResult = recv(m_Socket, inputBuffer, bytesReceived, 0);
		if (iResult > 0)
		{
			std::cout << "Server reply: " << std::string(inputBuffer, bytesReceived) << std::endl;
		}
		else if (0 == iResult)
		{
			std::cout << "Connection closed" << std::endl;
		}
		else
		{
			//TODO log receive error
		}
	} while (iResult > 0 && bytesReceived > 0);	
}

void CClient::PushTransaction(const std::string & singleTransaction)
{
	if (CheckTransaction(singleTransaction, '|'))
		dataToSend.push_back(singleTransaction);
}

void CClient::PushFile(const std::string & fileName)
{
	std::ifstream inputFile(fileName, std::ios::out);
	if (inputFile.is_open())
	{
		std::string transactionString{};
		while (std::getline(inputFile, transactionString))
		{
			dataMutex.lock();
			dataToSend.push_back(transactionString);
			dataMutex.unlock();
		}
		inputFile.close();
	}
}

bool CClient::CheckTransaction(const std::string & transaction, const char delimiter)
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

