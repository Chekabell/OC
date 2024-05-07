#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <Windows.h>

#pragma comment(lib,"ws2_32.lib")
#define BUFF_SIZE 400 

const int servPort = 4542;
const int clientPort = 4545;

DWORD WINAPI startServer() {
	WSADATA wsData;

	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (erStat) {
		std::cout << "Error WinSock version initialization #";
		std::cout << WSAGetLastError();
		return 1;
	}
	else
		std::cout << "WinSock initialization is OK" << std::endl;

	SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ServSock == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else
		std::cout << "Server socket initialization is OK" << std::endl;

	in_addr ip_to_num;
	erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (erStat <= 0) {
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return 1;
	}

	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(servPort);

	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
	if (erStat) {
		std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else
		std::cout << "Binding socket to Server info is OK" << std::endl;

	erStat = listen(ServSock, SOMAXCONN);

	if (erStat) {
		std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	else std::cout << "Listening..." << std::endl;

	sockaddr_in clientInfo;

	ZeroMemory(&clientInfo, sizeof(clientInfo));

	int clientInfo_size = sizeof(clientInfo);
	
	SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

	if (ClientConn == INVALID_SOCKET) {
		std::cout << "Client detected, but can't connect to a client. Error #" << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
		return 1;
	}
	else std::cout << "Connection to a client established successfully" << std::endl;

	std::vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
	short packet_size = 0;

	while (true) {
		packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
		std::cout << "Client message: " << servBuff.data() << std::endl;

		std::cout << "Your (host) message: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
			shutdown(ClientConn, SD_BOTH);
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 0;
		}

		packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
		
		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 1;
		}
	}

	closesocket(ServSock);
	closesocket(ClientConn);
	WSACleanup();
	return 0;
}

DWORD WINAPI startClient() {
	WSADATA wsData;

	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (erStat) {
		std::cout << "Error WinSock version initialization #";
		std::cout << WSAGetLastError();
		return 1;
	}
	else
		std::cout << "WinSock initialization is OK" << std::endl;

	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ClientSock == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}
	else
		std::cout << "Server socket initialization is OK" << std::endl;

	in_addr ip_to_num;
	erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (erStat <= 0) {
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return 1;
	}

	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(servPort);

	erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));
	if (erStat) {
		std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}
	else
		std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server" << std::endl;

	std::vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
	short packet_size = 0;

	while (true) {

		std::cout << "Your (Client) message to Server: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
			shutdown(ClientSock, SD_BOTH);
			closesocket(ClientSock);
			WSACleanup();
			return 0;
		}

		packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		}

		packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		}
		else
			std::cout << "Server message: " << servBuff.data() << std::endl;
	}

	closesocket(ClientSock);
	WSACleanup();

	return 0;
}

int main(void) {
	HANDLE server;
	HANDLE client;

	SECURITY_ATTRIBUTES serverSec;
	ZeroMemory(&serverSec, sizeof(serverSec));

	server = CreateThread(&serverSec,0,startServer,)

	SECURITY_ATTRIBUTES clientSec;
	ZeroMemory(&clientSec, sizeof(clientSec));

	CreateThread()

}