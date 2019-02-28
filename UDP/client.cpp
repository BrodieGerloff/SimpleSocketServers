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
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
	int sockFd, value;
	socklen_t size;
	const int portNumber = 8492;
	struct sockaddr_in serverAddr;
	char buffer[1024] = {0};
	string text;
	sockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockFd < 0) {
		perror("ERROR: failed to open socket");
		exit(1);
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	size = sizeof(serverAddr);
	
	cout << "Type 'exit' to terminate the connection" << endl;
	while(true) {		// Connection fully established, messages can be sent and received until 

		memset(&buffer, 0, sizeof(buffer));
		cout << "Please enter a message: ";				// "exit" is typed in to call a close() function
		getline(cin, text);
		strcpy(buffer, text.c_str());
		if(text == "exit"){
			//send a close command to server
			sendto(sockFd, (char*)buffer, 1024, MSG_CONFIRM, (struct sockaddr *) &serverAddr, size);
			break;
		}
		value = sendto(sockFd, (char*)buffer, 1024, MSG_CONFIRM, (struct sockaddr *) &serverAddr, size);
		if (value < 0) {
			perror("ERROR: Failed to send message");
			exit(1);
		}
		//Recieve Text
		memset(&buffer, 0, sizeof(buffer));
		value = recvfrom(sockFd, (char*)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &serverAddr, &size);
		buffer[value] = '\0';
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
