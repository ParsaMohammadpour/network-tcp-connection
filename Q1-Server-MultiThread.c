#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_CLIENT_NUMBER 10

struct ClientInfo {
   int clientId;
   struct sockaddr_in clientAddress;
   int clientSocket;
   char requestBuffer[100]; // In This Question It Is Not Used
   char responseBuffer[100];
   int isInUse;
};

struct ClientInfo clients[MAX_CLIENT_NUMBER];
pthread_t threads[MAX_CLIENT_NUMBER];

char * getTime(){
	time_t t;
	time(&t);
	return ctime(&t);
}

void *ClientHandler(void *threadId) {
	int id = threadId;
	// Printing Connection Details
	printf("--------------- Connection Number: %d ---------------\n", clients[id].clientId);
	printf("Thread Id %d Is Started.\n", id);
	printf("Client IP & Port: %s:%i\n", inet_ntoa(clients[id].clientAddress.sin_addr), ntohs(clients[id].clientAddress.sin_port));
	
	memset(clients[id].responseBuffer, '\0', sizeof(clients[id].responseBuffer));
	
	// Sending Response
	strcpy(clients[id].responseBuffer, getTime());
	if (send(clients[id].clientSocket, clients[id].responseBuffer, strlen(clients[id].responseBuffer), 0) < 0){
		printf("Error On Sending Time To Client!\n");
		exit(-1);
	}
	
	// This Line Is Only For Checking If Threading Is Working Properly Or Not
	// It make thread To Wait For 60 Seconds
	sleep(60);
	
	// Closing Client Socket
	close(clients[id].clientSocket);
    clients[id].isInUse = 0;
    pthread_exit(NULL);
}

int main(void)
{
	int maxClientNumber = 10; // Maximum Number Of Connection At Time
	int connectionCounter = 1;
	
	// Making Socket
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketDescriptor < 0){
        printf("Error On Socket Creation!\n");
        return -1;
    }
    printf("Pure Socket Is Created.\n");
	
    // Server Socket Information (Address & IP)
	struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(2000);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Setting Server Socket Information To Socket
    if(bind(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress))<0){
        printf("Error On Setting Socket Details!\n");
        return -1;
    }
    printf("Socket Details Was Added Successfuly\n");
	printf("Start Listening For Request...\n");
	
	int threadId = 0;
	for(int i = 0; i < MAX_CLIENT_NUMBER; i++){
		clients[i].isInUse = 0;
	}
    
    while(1){
		// Listening For Request
		if(listen(socketDescriptor, 1) < 0){
			printf("Error On Socket Listening!\n");
			return -1;
		}
		
		// Accepting Client request
		struct sockaddr_in clientAddress;
		int clientSize = sizeof(clientAddress);
		int clientSocket = accept(socketDescriptor, (struct sockaddr*)&clientAddress, &clientSize);
		if (clientSocket < 0){
			printf("Error On Accepting Client Request!\n");
		}
		threadId = -1;
		for(int i = 0; i < MAX_CLIENT_NUMBER; i++){
			if (clients[i].isInUse == 0){
				threadId = i;
				break;
			}
		}
		if (threadId < 0){
			printf("Max Client Number Is Reached!\n");
			for(int i = 0; i < MAX_CLIENT_NUMBER; i++)
				printf("Client Number: %d  -> Is In Use: %d\n", (i+1), clients[i].isInUse);
		}else {
			clients[threadId].isInUse = 1;
			clients[threadId].clientId = connectionCounter++;
			clients[threadId].clientAddress = clientAddress;
			clients[threadId].clientSocket = clientSocket;
			int rc = pthread_create(&threads[threadId], NULL, ClientHandler, (void *)threadId);
			if(rc){
				printf("Error On Making A Thread!\n");
				exit(-1);
			}else{
				//printf("Thread Id Is: %d\n", threadId);
			}
		}
	}
    close(socketDescriptor);
    return 0;
}