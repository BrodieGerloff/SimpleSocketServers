//Lab 1- TCP/UDP Server/ Client Chat
//By Brodie Gerloff and Jakob Meyer

//TCP Client:
//Simple client for a TCP server.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
	int sockFd, value;
	const int portNumber = 8492;
	struct sockaddr_in serverAddr, cliAddr;
	char buffer[1024] = {0};
	string text;
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd < 0) {
		perror("ERROR: failed to open socket");
		exit(1);
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);
	if (connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("ERROR: failed to connect to server");
		exit(1);
	}
	cout << "Type 'exit' to terminate the connection" << endl;
	while(true) {		// Connection fully established, messages can be sent and received until 

		memset(&buffer, 0, sizeof(buffer));
		cout << "Please enter a message: ";				// "exit" is typed in to call a close() function
		getline(cin, text);
		strcpy(buffer, text.c_str());
		if(text == "exit"){
			//send a close command to server
			send(sockFd, buffer, sizeof(buffer), 0);
			break;
		}
		value = send(sockFd, buffer, sizeof(buffer), 0);
		if (value < 0) {
			perror("ERROR: Failed to send message");
			exit(1);
		}
		//Recieve Text
		memset(&buffer, 0, sizeof(buffer));
		value = recv(sockFd, buffer, 1024, 0);
		if(!strcmp(buffer, "exit")){
			//Recieved close command
			cout << "Server closed." << endl;
			break;
		}
		if (value < 0) {
			perror("ERROR: Socket read failed");
			exit(1);
		}
		printf("Message from Server: %s\n", buffer);
		}
	close(sockFd);
	return 0;
}
