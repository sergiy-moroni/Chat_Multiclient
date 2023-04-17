#include <vector>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

SOCKET Connection;
using namespace std;
const char SERVER_IP[] = "10.211.55.3";
const short SERVER_PORT_NUM = 4700;
const short BUFF_SIZE = 1024;
int erStat;

//перевод из буера в строку для получения 
void vectostr(const vector<char>& vec, string& str)
{
	str.clear();
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == '\0')
		{
			break;
		}
		str.push_back(vec[i]);
	}
}
//перевод из строки в буфер для отправки
void strtovec(vector<char>& vec, const string& str)
{
	vec.clear();
	for (int i = 0; i < str.length(); i++)
	{
		vec.push_back(str[i]);
	}
	vec.push_back('\0');
}

void start(SOCKET ClientSock)
{
	vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
	short packet_size = 0;
	//Подключение успешно, сообщим об этом серверу

	string text = "!";
	strtovec(clientBuff, text);
	packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
	Sleep(100);
	//clientBuff = { '\0' };
	packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);
	cout << "Server message: " << servBuff.data() << endl;
	while (true) {
		char choise;
		cout << "Choise: 1-Enter, 2 - registration, 0 - exit" << endl;
		cin >> choise;
		clientBuff = { choise,'\0' };
		packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
		Sleep(100);

		if (choise != '0' && choise != '1' && choise != '2')
		{
			cout << "Incorrect Choise" << endl;
			continue;
		}
		//Выход
		if (choise == '0')
		{

			packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);
			cout << "Server message: " << servBuff.data() << endl;
			break;
		}
		//Регистрация пользователя
		if (choise == '2')
		{
			string name;
			string log;
			string pass;
			cout << "Registration: " << endl;
			cout << "Name?" << endl;
			getline(cin, name);
			getline(cin, name);
			strtovec(clientBuff, name);
			packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
			Sleep(100);
			cout << "Login?" << endl;
			getline(cin, log);
			strtovec(clientBuff, log);
			packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
			Sleep(100);
			cout << "Pass?" << endl;
			getline(cin, pass);
			strtovec(clientBuff, pass);
			packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
			Sleep(100);
			clientBuff = { '\0' };
			continue;
		}
		// Вход по логину и паролю
		if (choise == '1') {
			string log;
			string pass;
			cout << "Enter: " << endl;
			cout << "Login?" << endl;
			getline(cin, log);
			getline(cin, log);
			strtovec(clientBuff, log);
			packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
			Sleep(100);
			cout << "Pass?" << endl;
			getline(cin, pass);
			strtovec(clientBuff, pass);
			packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
			packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);
			cout << "Server message: " << servBuff.data() << endl;
			char c = servBuff[0];
			if (c == '0')
			{
				cout << "Incorrect log or pass" << endl;
				continue;
			}
			if (c == '1')
			{
				cout << "log pass is OK" << endl; 
				while (true) {
					char choise2;
					cout << "Choise: 2-Reead messages, 1 - send message, 0 - exit" << endl;
					cin >> choise2;

					clientBuff = { choise2 };
					packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
					Sleep(100);

					if (choise2 != '0' && choise2 != '1' && choise2 != '2') {
						cout << "Incorrect choise" << endl;
					}
					if (choise2 == '0') { break; }

					if (choise2 == '2') {
						packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);
						cout << "message: " << servBuff.data() << endl;
					}
					if (choise2 == '1') {
						string mess;
						cout << "Message: ";
						getline(cin, mess);
						getline(cin, mess);
						strtovec(clientBuff, mess);
						packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
						Sleep(100);
					}
				}
				continue;
			}
		}
		closesocket(ClientSock);
		WSACleanup();
	}
}


int main()
{
	cout << "1" << endl;
	setlocale(LC_ALL, "Rus");
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	cout << "2" << endl;
	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";

	std::thread t(start, Connection);

	t.join();

	return 0;
}
