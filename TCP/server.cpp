//Lab 1- TCP/UDP Server/ Client Chat
//By Brodie Gerloff and Jakob Myer

//TCP Server:
//Simple server for a TCP server.

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

/*
Things to note: need to use POSIX threads, create a helper function (that handles incoming messages).
Need an array of client file descriptors. e.g. int client[100]
Also an integer to keep track of number of connections. Lets call it "n"

A seperate helper function for sending an incoming message to all clients is also helpful. Can just loop through client[]

To use the posix threads, you need a function that houses your client communication code. This will be run in each thread. Basically,
Just loop your recv function as normal. You also need to remove a client from the array if it disconnects in this loop.

In your main function loop
*/

int main() {
	
	//Setup
	int clientSocket, serverSocket;
	int portNum = 8492;
	char buffer[1024];
	int bindServer;
	string chat;
	int recieve;

	//set
	sockaddr_in serverAddr;
	socklen_t size;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);
	serverAddr.sin_port = htons(portNum);

	//Creating the socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket < 0) {
		cout << "Error binding socket." << cout;
		return -1;
	}

	//Bind the server
	bindServer = bind(serverSocket, (struct sockaddr*) &serverAddr, 
        sizeof(serverAddr));

	if(bindServer < 0)
	{
		cout << "Error binding socket" << endl;
		return -1;
	}

	
	//Listen for client connections

	size = sizeof(serverAddr);

	listen(serverSocket, 5);

	//Accept connection

	clientSocket = accept(serverSocket, (sockaddr *)&serverAddr, &size);
	
	if (clientSocket < 0) {
		cout << "Error connecting to client." << endl;
		return -1;
	}

	//Chat loop
	
	cout << "Connection made! Type 'exit' to quit." << endl;

	while(true) {


		//Client Chat
		memset(&buffer, 0, sizeof(buffer)); 				//Clear mem for client
		cout << "Awaiting client:" << endl;
		
		recieve = recv(clientSocket, (char*)&buffer, sizeof(buffer), 0); 
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
			send(clientSocket, (char*)&buffer, sizeof(buffer), 0);
			break;
		}

		//Sends the message
		recieve = send(clientSocket, (char*)&buffer, sizeof(buffer), 0);
	}

	//Close Connection
	close(clientSocket);
	close(serverSocket);


	return 0;
}
