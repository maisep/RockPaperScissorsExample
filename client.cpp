#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <cstdlib>

#define MAX_SIZE 1024

SOCKET client_sock;

void rps_recv() {
	char buf[MAX_SIZE] = { };
	while (1) {
		ZeroMemory(&buf, MAX_SIZE);
		if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
			std::cout << "상대방 : " << buf << std::endl;
			break;
		}
	}
}

int main() {
	WSADATA wsa;
	int code = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (!code) {
		client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		SOCKADDR_IN client_addr = {};
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(7777);
		InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

		while (1) {
			if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
				std::cout << "connect" << std::endl;
				break;
			}
			std::cout << "connect error" << std::endl;
		}

		char my[MAX_SIZE] = { };
		char msg[MAX_SIZE] = { };

		while (1) {
			std::cout << "\n가위 : 0, 바위 : 1, 보 : 2" << std::endl;
			ZeroMemory(&msg, MAX_SIZE);
			std::cin >> my;
			send(client_sock, my, strlen(my), 0);
			rps_recv();
			system("cls");
			std::cout << "나의 선택 : " << my << "\n" << std::endl;
			recv(client_sock, msg, MAX_SIZE, 0);
			std::cout << msg << std::endl;
		}
		closesocket(client_sock);
	}

	WSACleanup();
	return 0;
}