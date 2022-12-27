#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    // Create socket:
    int socketDescription = socket(AF_INET, SOCK_STREAM, 0);
    if(socketDescription < 0){
        printf("Error On Creating Socket!\n");
        return -1;
    }
    printf("Socket Was Created.\n");
    
    // Set Server Details (Address & IP) To Get Connected
	struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(2000);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Asking Server For A Connection
    if(connect(socketDescription, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        printf("Error On Asking Server For A Connection!\n");
        return -1;
    }
    printf("Connected To Server.\n");
	
	// Defining A Buffer For Receiving Server Response
	char responseBuffer[100];
    memset(responseBuffer,'\0',sizeof(responseBuffer));
    
    // Receive Time From Server
    if(recv(socketDescription, responseBuffer, sizeof(responseBuffer), 0) < 0){
        printf("Error On Receiving Time From Server!\n");
        return -1;
    }
    printf("Server's response: %s\n",responseBuffer);
    
    // Closing Socket:
    close(socketDescription);
    return 0;
}