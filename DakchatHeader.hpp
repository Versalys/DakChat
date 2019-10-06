#include <ncurses.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string.h>
#include <stdio.h>      //for printf() and fprintf()
#include <sys/socket.h> //for socket(), connect(), send(), and recv()
#include <arpa/inet.h>  //for sockaddr_in and inet_addr()
#include <unistd.h>     //for close()
#include <iostream>

#define MESSAGESIZE 256
#define PORT 86

struct dakUser
{
    char username[16];
    int sock;
    int color;
    struct sockaddr_in clntAddr;
};

struct dakMessage
{
    char username[16];
    int color;
    char message[MESSAGESIZE];
};



/*
void titleCard(){

    cout " _______    ______   __    __   ______   __    __   ______  ________ "
    cout "|       \\  /      \ |  \  /  \ /      \ |  \  |  \ /      \|        \"
    cout "| $$$$$$$\|  $$$$$$\| $$ /  $$|  $$$$$$\| $$  | $$|  $$$$$$\\$$$$$$$$"
    cout "| $$  | $$| $$__| $$| $$/  $$ | $$   \$$| $$__| $$| $$__| $$  | $$   "
    cout "| $$  | $$| $$    $$| $$  $$  | $$      | $$    $$| $$    $$  | $$   "
    cout "| $$  | $$| $$$$$$$$| $$$$$\  | $$   __ | $$$$$$$$| $$$$$$$$  | $$   "
    cout "| $$__/ $$| $$  | $$| $$ \$$\ | $$__/  \| $$  | $$| $$  | $$  | $$   "
    cout "| $$    $$| $$  | $$| $$  \$$\ \$$    $$| $$  | $$| $$  | $$  | $$   "
    cout " \$$$$$$$  \$$   \$$ \$$   \$$  \$$$$$$  \$$   \$$ \$$   \$$   \$$    ""
}
*/
