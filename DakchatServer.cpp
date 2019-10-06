#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string.h>
#include <stdio.h>      //for printf() and fprintf()
#include <sys/socket.h> //for socket(), connect(), send(), and recv()
#include <arpa/inet.h>  //for sockaddr_in and inet_addr()
#include <unistd.h>     //for close()
#include <iostream>
#include "DakchatHeader.hpp"

#define MAXPENDING 6
#define RCVBUFSIZE 32

void getUsername(struct dakUser&);
void addColorCodes(struct dakUser&);
void recvSendChat(struct dakUser&);
void handleTCPClient(int clntSocket);

/*
TODO:

TEST BOTH IMPLEMENTATIONS
REMEMBER TO USE F6 AND CONFIG NEW COMPILATION INTERFACE
*/

int main(){
    int servSock;
    struct dakUser servUser;
    struct sockaddr_in servAddr;
    unsigned int clntLen;
    unsigned int nextSock = 0;

    std::cout << "WELCOME TO DAKCHAT SERVER!!! \n Tip: Remember to run as root. \n";

    if((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        fprintf(stderr, "socket() failed... \n");
        exit(1);
    }
    //std::cout << servSock << std::endl;

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);

    if(bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
        fprintf(stderr, "bind() failed... \n");
        std::cout << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "listening on port " << PORT << "\n";

    if(listen(servSock, MAXPENDING) < 0){
        fprintf(stderr, "listen() failed \n");
        std::cout << strerror(errno) << std::endl;
        exit(1);
    }

    for(;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        clntLen = sizeof(servUser.clntAddr);
        if(servUser.sock = accept(servSock, (struct sockaddr *) &servUser.clntAddr, &clntLen) < 0){
            printf("accept() failed\n");
            std::cout << strerror(errno) << std::endl;
        } else {
            printf("accept() succeded");
            getUsername(servUser);
            addColorCodes(servUser);
        }


        recvSendChat(servUser);



        //handleTCPClient(clntSock);
    }
    close(servSock);
}

void getUsername(struct dakUser &user){
    int totalBytesRcvd = 0;
    int bytesRcvd;
    do  //changed because recv() keeps failing. Due to no loop?
    {
        if(bytesRcvd = recv(user.sock, user.username, 15, 0)) < 0)
        {
        printf("recv() failed or connection closed prematurely\n");
        std::cout << strerror(errno) << std::endl;
        totalBytesRcvd += bytesRcvd;
        }
    }while(bytesRcvd )
    user.username[totalBytesRcvd] = '\0';
}

void addColorCodes(struct dakUser& user){
    bool *colorTaken = new bool[7];
    int iterator = 0;
    while(iterator < 7){
        if(colorTaken[iterator] == true)
            iterator++;
        else
            user.color = iterator + 1;
        }
}

void recvSendChat(struct dakUser& user){
    struct dakMessage myMessage;
    strcpy(myMessage.username, user.username);

    int totalBytesRcvd = 0;
    int bytesRcvd;
    if((bytesRcvd = recv(user.sock, myMessage.message, MESSAGESIZE, 0)) <= 0)
        printf("recv() failed or connection closed prematurely\n");
    totalBytesRcvd += bytesRcvd;
    myMessage.message[totalBytesRcvd] = '\0';

    if(send(user.sock, &myMessage, sizeof(myMessage), 0) != sizeof(myMessage))
        printf("sent a different number of bytes that expected");
    else
        printf(myMessage.username, ": ", myMessage.message);
}

void handleTCPClient(int clntSocket){
    char buffer[RCVBUFSIZE];
    int recvMsgSize;
}
