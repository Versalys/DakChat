/*
    DAKCHAT
    CREATED BY AIDAN ERICKSON FOR INTERNSHIPS
    GITHUB/STACKOVERFLOW : VERSALYS

    CLIENT-SERVER CHAT SYSTEM PROJECT.
*/

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
#include "DakchatHeader.hpp"
/*  TODO list
    1. NO INPUT DUE TO NO SERVER. START CREATING SERVER AND INSTALL VIRTUAL MACHINE
    2.
*/

WINDOW* create_newwin(int height, int width, int starty, int startx);  //creates win based on parameters
void displayMessage(char*, int&, WINDOW*, int&);  //displays message at int line
void startChat(int&);  //starts chat process

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in servAddr;
    unsigned short servPort;
    char* servIP;

    char* username = new char[16];
    int color;
    if(argc != 2){
        fprintf(stderr, "USAGE %s <server IP> \n");
        exit(1);
    }
    servIP = argv[1];
    servPort = PORT;

    if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "socket() failed");
        exit(1);
    }
    //construct networking structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);
    servAddr.sin_port = htons(servPort);

    std::cout << "WELCOME TO DAKCHAT!!!" << std::endl;
    //username and color selection
    std::cout << "Username? (up to 15 characters no spaces) :";
    std::cin.get(username, 15);
    std::cout << std::endl << "You chose username: " << *username;

    if(connect(sock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0){
        fprintf(stderr, "connect() failed");
        exit(1);
    }


    if(send(sock, username, sizeof(*username), 0) != sizeof(*username)){
        fprintf(stderr, "send() sent a different number of bytes than expected");
        exit(1);
    }

    startChat(sock);
}

void startChat(int& sock){
    WINDOW *chatWin, *displayWin;
	int starty, height, displayLine = 1;

	initscr();

	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

    cbreak();
	height = 6;
	starty = LINES - height;
	refresh();
	chatWin = create_newwin(height, COLS, starty, 0);

    height = LINES - height;
    starty = 0;
    displayWin = create_newwin(height, COLS, starty, 0);

    mvwprintw(chatWin, 1, 1, "CHAT: ");
    refresh();
    char* cStr = new char[MESSAGESIZE];
    for(;;){
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        wmove(chatWin, 1, 7);
        if(wgetstr(chatWin, cStr) == ERR)
            printw("errorthrown");
        if(strcmp(cStr, "") != 0)
        {
            if(strcmp(cStr, ":quit") == 0)
                break;
            else
            {
                displayMessage(cStr, displayLine, displayWin, sock);
                wclear(chatWin);
                box(chatWin, 0, 0);
                mvwprintw(chatWin, 1, 1, "CHAT: ");
                wmove(chatWin, 1, 7);
            }
        }

    }

	endwin();			/* End curses mode		  */
	return;
}

void displayMessage(char* message, int &line, WINDOW* display, int& sock){

    struct dakMessage incomingMessage;

    if(send(sock, message, sizeof(*message), 0) != sizeof(*message)){
        fprintf(stderr, "send() sent a different number of bytes than expected");
        exit(1);
    }

    if(recv(sock, &incomingMessage, sizeof(incomingMessage), 0) < 0){
        fprintf(stderr, "recv() failed");
        exit(1);
    }

    incomingMessage.message[sizeof(incomingMessage.message)] = '\0';

    if(line < 20){
        mvwprintw(display, line, 1, incomingMessage.message);
        wrefresh(display);
        line++;
    } //ADD SUPPORT FOR MULTILINE MESSAGES

}

WINDOW* create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}
