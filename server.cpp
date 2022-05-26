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
		std::cout << "�����ϴ�." << std::endl;
		draw_count++;
		return "�����ϴ�.";
	}
	else if (my_n > opp_n) {
		if (my_n == 2 && opp_n == 0) {
			std::cout << "�����ϴ�." << std::endl;
			lose_count++;
			return "�̰���ϴ�.";
		}
		else {
			std::cout << "�̰���ϴ�." << std::endl;
			win_count++;
			return "�����ϴ�.";
		}
	}
	else {
		if (my_n == 0 && opp_n == 2) {
			std::cout << "�̰���ϴ�." << std::endl;
			win_count++;
			return "�����ϴ�.";
		}
		else {
			std::cout << "�����ϴ�." << std::endl;
			lose_count++;
			return "�̰���ϴ�.";
		}
	}
}

std::string rps_recv() {
	char buf[MAX_SIZE] = { };
	while (1) {
		ZeroMemory(&buf, MAX_SIZE);
		if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
			std::cout << "���� : " << buf << std::endl;
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
			std::cout << "\n���� : 0, ���� : 1, �� : 2" << std::endl;
			std::cin >> my;
			send(client_sock, my, strlen(my), 0);
			opp = rps_recv();
			system("cls");
			std::cout << "���� ���� : " << my << "\n" << std::endl;
			result = check(my, opp);
			std::cout << "���� ���� : " << win_count << "�� " << draw_count << "�� " << lose_count << "��" << std::endl;
			result += "\n���� ���� : " + std::to_string(lose_count) + "�� " + std::to_string(draw_count) + "�� " + std::to_string(win_count) + "��";
			send(client_sock, result.c_str(), result.length(), 0);
		}
		closesocket(client_sock);
		closesocket(server_sock);
	}

	WSACleanup();

	return 0;
}