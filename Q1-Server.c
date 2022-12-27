#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

char * getTime(){
	time_t t;
	time(&t);
	return ctime(&t);
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
			return -1;
		}
		
		// Printing Connection Details
		printf("--------------- Connection Number: %d ---------------\n", connectionCounter++);
		printf("Client IP & Port: %s:%i\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
		
		// Making Buffer For Sending Response
		char responseBuffer[100];
		memset(responseBuffer, '\0', sizeof(responseBuffer));
		
		// Sending Response
		strcpy(responseBuffer, getTime());
		if (send(clientSocket, responseBuffer, strlen(responseBuffer), 0) < 0){
			printf("Error On Sending Time To Client!\n");
			return -1;
		}
		
		// Closing Client Socket:
		close(clientSocket);
	}
    close(socketDescriptor);
    return 0;
}