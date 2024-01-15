#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	// 1. Socket ���� �۾�
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl;
		exit(-1);
	}

	// 2. Server ���� �ּҰ� �ۼ� �� �ʱ�ȭ
	// SOCKADDR_IN �� ����� ��, (�빮��)����� �����츸 �����մϴ�.
	// struct sockaddr_in �̷��� �ۼ��� ����� C ���� �����ϱ⿡ �� ����� �� �����ϴ�.
	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));

	// 3. Server ���� �ּ� ����
	// AF_INET6 �� IPv4 �� IPv6 ��� ��� �����մϴ�.
	ServerSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET6, "127.0.0.1", &(ServerSockAddr.sin_addr.s_addr));
	ServerSockAddr.sin_port = htons(5001);

	// 4. Server ���� ���� �� ����
	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 5. Server ���� ���� �۾�
	Result = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Result == SOCKET_ERROR)
	{
		cout << "can't connect. : " << GetLastError() << endl;
		exit(-1);
	}

	// 6. Server �� ���� ����(Buffer) �ۼ�
	const char Message[] = "give me message.";
	int SentByte = send(ServerSocket, Message, (int)strlen(Message), 0);

	cout << "Send Data" << endl;
	
	// 7. Server �κ��� ���� ����(Buffer) �ۼ�
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0);

	cout << "Server Sent : " << RecvBuffer << endl;

	// 8. ���� ���� �� ����
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}