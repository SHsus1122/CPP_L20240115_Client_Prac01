#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32")

#pragma pack(push, 1)

typedef struct _Data
{
	int FirstNumber;
	int SecondNumber;
	char Operator;
} Data;
#pragma pack(pop)

int main()
{
	// 1. 랜덤 난수를 위해서 srand 를 선언해줍니다.
	srand((u_int)(time(nullptr)));

	// 2. 소켓 연결 준비
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 3. 소켓 생성 및 설정
	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 4. 소켓 주소 생성 및 설정
	//		- 소켓 주소 생성
	//		- 소켓 초기값 0 으로 초기화
	//		- 주소 체계를 IPv4 로 설정
	//		- IPv4 방식, IP 주소를 "127.0.0.1" 으로, 네트워크 주소 정보 저장
	//		- 소켓 주소의 포트를 "5001" 번으로 지정
	struct sockaddr_in ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(ListenSockAddr.sin_addr.s_addr));
	ListenSockAddr.sin_port = htons(5001);

	// 5. 서버로 날리기 위한 소켓 연결 작업 입니다.
	//	- bind : 소켓을 특정 주소에 연결하는 함수
	//	- 첫 번째 인자 : 서버 소켓
	//	- 두 번째 인자 : 구조체의 주소를 소켓 API 가 요구하는 struct sockaddr 형태로 형변환하여 전달
	//	- 세 번째 인자 : 구조체의 크기
	bind(ListenSocket, (struct sockaddr*)&ListenSockAddr, sizeof(ListenSockAddr));

	// 소켓이 들어오는 연결을 받아 들일 수 있는 상태로 전환, 최대 5개의 연결 동시처리 가능
	listen(ListenSocket, 5);

	// 6. 클라이언트용 소켓 생성 및 설정
	//	- 소켓 주소 구조체 생성
	//	- 소켓 주소 초기화
	//	- 소켓 주소의 길이값을 담을 변수 생성
	//		- 소켓 생성, accept : 서버 소켓에서 클라이언트의 연결을 수락하는 역할
	//		- 서버 소켓
	//		- 클라이언트의 주소 정보를 받아올 구조체 포인터 전달
	//		- 구조체의 크기가 동적이기 때문에 포인터 방식으로 전달
	struct sockaddr_in ClinetSockAddr;
	memset(&ClinetSockAddr, 0, sizeof(ClinetSockAddr));
	int ClientSockAddrLength = sizeof(ClinetSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (struct sockaddr*)&ClinetSockAddr, &ClientSockAddrLength);

	while (true)
	{
		// 서버의 구조체와 동일한 구조의 패킷 방식 구조체 사용
		Data Packet;
		
		Packet.FirstNumber = (rand() % 20000) - 10000;	// 0 ~ 9999
		Packet.SecondNumber = (rand() % 20000) - 10000;	// 1 ~ 9999
		Packet.SecondNumber == 0 ? 1 : Packet.SecondNumber;

		Packet.Operator = rand() % 5;					// 0 ~ 4

		// 7. 구조체 데이터를 전달
		send(ClientSocket, (char*)&Packet, (u_int)sizeof(Packet), 0);

		// 8. 구조체 데이터 받기
		char Buffer[1024] = { 0, };
		int RecvByte = recv(ClientSocket, Buffer, 1024, 0);
		if (RecvByte <= 0)
		{
			break;
		}

		// 9. 결과를 받아서 출력
		long long Result = 0;
		// memcpy : 메모리으 값을 복사하는 기능을 하는 함수입니다.
		// 복사 받을 메모리를 가리키는 포인터, 복사할 메모리를 가리키는 포인터, 복사할 데이터(값) 의 길이(바이트 단위)
		memcpy(&Result, Buffer, sizeof(Result));
		cout << Result << endl;
	}
	// 10. 소켓 연결 닫기
	closesocket(ClientSocket);
	closesocket(ListenSocket);

	// 11. Winsock 2 DLL(Ws2_32.dll) 사용 종료, 원속 라이브러리 사용 종료
	WSACleanup();

	return 0;
}