#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <Windows.h>

//struct CI {
//	SOCKET sock;
//	char* ip[30];
//};

#pragma comment(lib,"ws2_32.lib")
#define BUFF_SIZE 512

std::vector<SOCKET> clientsSocket;

DWORD WINAPI startClientThread(LPVOID lpParam) {
	//CI* client = (CI*)lpParam;
	SOCKET client = (SOCKET)lpParam;
	char buffer[BUFF_SIZE];
	while (true) {
		int packet_size = recv(client, buffer, BUFF_SIZE, 0);
		//strcat_s(*client->ip, 482, *buffer);

		if (packet_size == INVALID_SOCKET) {
			closesocket(client);
			break;
		}

		for (SOCKET otherClientSocket : clientsSocket) {
			if (otherClientSocket != client) {
				send(otherClientSocket, buffer, packet_size, 0);
			}
		}
	}
	clientsSocket.erase(std::remove(clientsSocket.begin(), clientsSocket.end(), client), clientsSocket.end());
	return 0;
}

int main(void) {
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
	servInfo.sin_port = htons(4452);

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

	while (true) {
		sockaddr_in clientInfo;
		ZeroMemory(&clientInfo, sizeof(clientInfo));
		int clientInfo_size = sizeof(clientInfo);

		SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
		//CI cli;
		//cli.sock = ClientConn;

		if (ClientConn == INVALID_SOCKET) {
			std::cout << "Client detected, but can't connect to a client. Error #" << WSAGetLastError() << std::endl;
			closesocket(ServSock);
			WSACleanup();
			return 1;
		}
		else{
			std::cout << "Client connection established successfully" << std::endl;

			char clientIP[22];
			inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);
			//strcat_s(clientIP, 30, (char*)clientInfo.sin_port);
			//strcpy_s(*cli.ip, 22, clientIP);
			std::cout << "Client IP: " << clientIP << ":" << clientInfo.sin_port << std::endl;

			clientsSocket.push_back(ClientConn);
			CreateThread(NULL, 0, startClientThread, (LPVOID)ClientConn, NULL, NULL);
		}
	}
	closesocket(ServSock);
	WSACleanup();
	return 0;

}