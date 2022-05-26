#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <string>
#include <iostream>
#include <cstdlib>

#define MAX_SIZE 1024

SOCKET server_sock, client_sock;
int win_count = 0;
int lose_count = 0;
int draw_count = 0;

std::string check(std::string my, std::string opp) {
	int my_n = stoi(my);
	int opp_n = stoi(opp);

	if (my_n == opp_n) {
		std::cout << "비겼습니다." << std::endl;
		draw_count++;
		return "비겼습니다.";
	}
	else if (my_n > opp_n) {
		if (my_n == 2 && opp_n == 0) {
			std::cout << "졌습니다." << std::endl;
			lose_count++;
			return "이겼습니다.";
		}
		else {
			std::cout << "이겼습니다." << std::endl;
			win_count++;
			return "졌습니다.";
		}
	}
	else {
		if (my_n == 0 && opp_n == 2) {
			std::cout << "이겼습니다." << std::endl;
			win_count++;
			return "졌습니다.";
		}
		else {
			std::cout << "졌습니다." << std::endl;
			lose_count++;
			return "이겼습니다.";
		}
	}
}

std::string rps_recv() {
	char buf[MAX_SIZE] = { };
	while (1) {
		ZeroMemory(&buf, MAX_SIZE);
		if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
			std::cout << "상대방 : " << buf << std::endl;
			break;
		}
	}

	return buf;
}

int main() {
	WSADATA wsa;

	int code = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (!code) {
		server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		SOCKADDR_IN server_addr = {};
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(7777);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr));
		listen(server_sock, SOMAXCONN);

		std::cout << "server on" << std::endl;

		SOCKADDR_IN client_addr = {};
		int cssize = sizeof(client_addr);
		ZeroMemory(&client_addr, cssize);
		client_sock = accept(server_sock, (sockaddr*)&client_addr, &cssize);

		char my[MAX_SIZE] = { };
		std::string opp = "";
		std::string result = "";

		while (1) {
			std::cout << "\n가위 : 0, 바위 : 1, 보 : 2" << std::endl;
			std::cin >> my;
			send(client_sock, my, strlen(my), 0);
			opp = rps_recv();
			system("cls");
			std::cout << "나의 선택 : " << my << "\n" << std::endl;
			result = check(my, opp);
			std::cout << "현재 성적 : " << win_count << "승 " << draw_count << "무 " << lose_count << "패" << std::endl;
			result += "\n현재 성적 : " + std::to_string(lose_count) + "승 " + std::to_string(draw_count) + "무 " + std::to_string(win_count) + "패";
			send(client_sock, result.c_str(), result.length(), 0);
		}
		closesocket(client_sock);
		closesocket(server_sock);
	}

	WSACleanup();

	return 0;
}