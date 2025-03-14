#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#define PORT 8080 

using namespace std;

int main(int argc, char const *argv[]) 
{ 
    int socket_fd = 0; 
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0}; 
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        printf("\n socket error \n"); 
        return -1; 
    } 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nserver error \n"); 
        return -1; 
    } 
   
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        printf("\nconnection \n"); 
        return -1; 
    }
    string sendString;
    string doString;
    char *sendMsg;
    do{
        cout << "Send: " <<endl;
        cin >> sendString;
        sendMsg = (char*)sendString.c_str();        
        send(socket_fd , sendMsg , strlen(sendMsg) , 0 );
        read( socket_fd , buffer, 1024); 
        printf("received : %s\n",buffer );
        cout << "Send another message(y/n): ";
        cin >> doString;
    }while(doString == "Y" || doString == "y");
     
    return 0; 
} 