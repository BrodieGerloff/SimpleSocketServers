//Lab 1- TCP/UDP Server/ Client Chat
//By Brodie Gerloff and Jakob Myer

//TCP Server:
//Simple server for a UDP server.

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main() {
	
	//Setup
	int serverSocket;
	int portNum = 8492;
	int bufSize = 1024;
	char buffer[bufSize];
	int bindServer;
	string chat;
	int recieve;

	//set
	sockaddr_in serverAddr, clientAddr;
	socklen_t serverSize, clientSize;

	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&clientAddr, 0, sizeof(clientAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(portNum);

	//Creating the socket
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (serverSocket < 0) {
		cout << "Error binding socket." << cout;
		return -1;
	}

	serverSize = sizeof(serverAddr);
	clientSize = sizeof(clientAddr);

	//Bind the server
	bindServer = bind(serverSocket, (struct sockaddr*) &serverAddr, 
        sizeof(serverAddr));

	if(bindServer < 0)
	{
		cout << "Error binding socket" << endl;
		return -1;
	}



	//Chat loop
	
	cout << "Connection made! Type 'exit' to quit." << endl;

	while(true) {


		//Client Chat
		memset(&buffer, 0, sizeof(buffer)); 				//Clear mem for client
		cout << "Awaiting client:" << endl;
		
		recieve = recvfrom(serverSocket, (char*)buffer, bufSize, MSG_WAITALL, (struct sockaddr *) &clientAddr, &clientSize);
		buffer[recieve] = '\0';
		if(!strcmp(buffer, "exit")) {
			cout << "Client has quit." << endl;
			break;
		}
		
		printf("Client : %s\n", buffer);

		//Server chat
		cout << "> ";
		getline(cin, chat);
		memset(&buffer, 0, sizeof(buffer)); 				//Clear mem for server
		strcpy(buffer, chat.c_str());
		if(chat == "exit") {
			recieve = sendto(serverSocket, (char*)buffer, bufSize, MSG_CONFIRM, (struct sockaddr *) &clientAddr, clientSize);
			break;
		}

		//Sends the message
		recieve = sendto(serverSocket, (char*)buffer, bufSize, MSG_CONFIRM, (struct sockaddr *) &clientAddr, clientSize);
	}

	//Close Connection
	close(serverSocket);


	return 0;
}
