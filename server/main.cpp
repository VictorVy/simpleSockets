#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		cerr << "Socketless.. :(" << endl;
		return -1;
	}

	sockaddr_in addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(55555);
//	inet_pton(AF_INET, "192.9.141.123", &addr.sin_addr);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sock, (sockaddr*) &addr, sizeof(addr)) < 0) {
		cerr << "Unbound... :(" << endl;
		return -2;	
	}

	if (listen(sock, 1) < 0) {
		cerr << "Deaf... :(" << endl;
		return -3;
	}

	sockaddr_in cAddr;
	socklen_t cSize = sizeof(cAddr);
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
	memset(host, 0, NI_MAXHOST);
	memset(serv, 0, NI_MAXSERV);

	int cSock = accept(sock, (sockaddr*) &addr, &cSize);

	if (cSock < 0) {
		cerr << "Unacceptable... :(" << endl;
		return -4;
	}

	close(sock);

	int result = getnameinfo((sockaddr*) &cAddr, sizeof(cAddr), host, NI_MAXHOST, serv, NI_MAXSERV, 0);

	if (result) {
		cout << string(host, NI_MAXHOST) << " connected on " << string(serv, NI_MAXSERV) << endl;
	} else {
		inet_ntop(AF_INET, &cAddr.sin_addr, host, NI_MAXHOST);
		cout << string(host, NI_MAXHOST) << " connected on " << ntohs(cAddr.sin_port) << endl;
	}

	size_t buffSize = 4096;
	char buff[buffSize];

	while (true) {
		memset(buff, 0, buffSize);

		int bytes = recv(cSock, buff, buffSize, 0);

		if (bytes == -1) {
			cerr << "Unbit... :(" << endl;
			break;
		}
		if (bytes == 0) {
			cout << "DCed" << endl;
			break;
		}

		string input = string(buff, 0, bytes);
		cout << "Received: " << input << endl;

		string response;

		if (input == "Hello?") {
			response = "World!";
		} else {
			response = "Huh?";
		}

		send(cSock, response.c_str(), response.size() + 1, 0);
	}

	close(cSock);
	
	return 0;	
}
