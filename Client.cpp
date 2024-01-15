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
	// 1. Socket 시작 작업
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl;
		exit(-1);
	}

	// 2. Server 소켓 주소값 작성 및 초기화
	// SOCKADDR_IN 로 사용한 즉, (대문자)방식은 윈도우만 지원합니다.
	// struct sockaddr_in 이렇게 작성한 방식은 C 언어에서 지원하기에 이 방식이 더 좋습니다.
	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));

	// 3. Server 소켓 주소 설정
	// AF_INET6 는 IPv4 와 IPv6 모두 사용 가능합니다.
	ServerSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET6, "127.0.0.1", &(ServerSockAddr.sin_addr.s_addr));
	ServerSockAddr.sin_port = htons(5001);

	// 4. Server 소켓 생성 및 설정
	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 5. Server 소켓 연결 작업
	Result = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Result == SOCKET_ERROR)
	{
		cout << "can't connect. : " << GetLastError() << endl;
		exit(-1);
	}

	// 6. Server 로 보낼 내용(Buffer) 작성
	const char Message[] = "give me message.";
	int SentByte = send(ServerSocket, Message, (int)strlen(Message), 0);

	cout << "Send Data" << endl;
	
	// 7. Server 로부터 받을 내용(Buffer) 작성
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0);

	cout << "Server Sent : " << RecvBuffer << endl;

	// 8. 소켓 종료 및 정리
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}