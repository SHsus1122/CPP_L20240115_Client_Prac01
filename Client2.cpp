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
	// 1. ���� ������ ���ؼ� srand �� �������ݴϴ�.
	srand((u_int)(time(nullptr)));

	// 2. ���� ���� �غ�
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 3. ���� ���� �� ����
	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 4. ���� �ּ� ���� �� ����
	//		- ���� �ּ� ����
	//		- ���� �ʱⰪ 0 ���� �ʱ�ȭ
	//		- �ּ� ü�踦 IPv4 �� ����
	//		- IPv4 ���, IP �ּҸ� "127.0.0.1" ����, ��Ʈ��ũ �ּ� ���� ����
	//		- ���� �ּ��� ��Ʈ�� "5001" ������ ����
	struct sockaddr_in ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(ListenSockAddr.sin_addr.s_addr));
	ListenSockAddr.sin_port = htons(5001);

	// 5. ������ ������ ���� ���� ���� �۾� �Դϴ�.
	//	- bind : ������ Ư�� �ּҿ� �����ϴ� �Լ�
	//	- ù ��° ���� : ���� ����
	//	- �� ��° ���� : ����ü�� �ּҸ� ���� API �� �䱸�ϴ� struct sockaddr ���·� ����ȯ�Ͽ� ����
	//	- �� ��° ���� : ����ü�� ũ��
	bind(ListenSocket, (struct sockaddr*)&ListenSockAddr, sizeof(ListenSockAddr));

	// ������ ������ ������ �޾� ���� �� �ִ� ���·� ��ȯ, �ִ� 5���� ���� ����ó�� ����
	listen(ListenSocket, 5);

	// 6. Ŭ���̾�Ʈ�� ���� ���� �� ����
	//	- ���� �ּ� ����ü ����
	//	- ���� �ּ� �ʱ�ȭ
	//	- ���� �ּ��� ���̰��� ���� ���� ����
	//		- ���� ����, accept : ���� ���Ͽ��� Ŭ���̾�Ʈ�� ������ �����ϴ� ����
	//		- ���� ����
	//		- Ŭ���̾�Ʈ�� �ּ� ������ �޾ƿ� ����ü ������ ����
	//		- ����ü�� ũ�Ⱑ �����̱� ������ ������ ������� ����
	struct sockaddr_in ClinetSockAddr;
	memset(&ClinetSockAddr, 0, sizeof(ClinetSockAddr));
	int ClientSockAddrLength = sizeof(ClinetSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (struct sockaddr*)&ClinetSockAddr, &ClientSockAddrLength);

	while (true)
	{
		// ������ ����ü�� ������ ������ ��Ŷ ��� ����ü ���
		Data Packet;
		
		Packet.FirstNumber = (rand() % 20000) - 10000;	// 0 ~ 9999
		Packet.SecondNumber = (rand() % 20000) - 10000;	// 1 ~ 9999
		Packet.SecondNumber == 0 ? 1 : Packet.SecondNumber;

		Packet.Operator = rand() % 5;					// 0 ~ 4

		// 7. ����ü �����͸� ����
		send(ClientSocket, (char*)&Packet, (u_int)sizeof(Packet), 0);

		// 8. ����ü ������ �ޱ�
		char Buffer[1024] = { 0, };
		int RecvByte = recv(ClientSocket, Buffer, 1024, 0);
		if (RecvByte <= 0)
		{
			break;
		}

		// 9. ����� �޾Ƽ� ���
		long long Result = 0;
		// memcpy : �޸��� ���� �����ϴ� ����� �ϴ� �Լ��Դϴ�.
		// ���� ���� �޸𸮸� ����Ű�� ������, ������ �޸𸮸� ����Ű�� ������, ������ ������(��) �� ����(����Ʈ ����)
		memcpy(&Result, Buffer, sizeof(Result));
		cout << Result << endl;
	}
	// 10. ���� ���� �ݱ�
	closesocket(ClientSocket);
	closesocket(ListenSocket);

	// 11. Winsock 2 DLL(Ws2_32.dll) ��� ����, ���� ���̺귯�� ��� ����
	WSACleanup();

	return 0;
}