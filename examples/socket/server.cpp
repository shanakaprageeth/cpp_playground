// Copyright 2019 Shanaka Prageeth
#include <iostream>
#include <string>
#include "socket/SocketServer.hpp"

// Use specific using declarations instead of using-directive
using std::cin;
using std::cout;
using std::endl;
using std::string;

// Entry point of the program: initializes and runs the server
int main() {
    char buffer[1024] = {0};

    SocketServer myserver;
    myserver.initSocket();
    myserver.setSocket();
    myserver.bindAndListen();
    myserver.acceptClients();
    string sendString;
    string doString;
    do {
        cout << "Send: " << endl;
        cin >> sendString;
        const char *sendMsg = sendString.c_str();
        myserver.readAndSend(buffer, sendMsg);
        cout << "Send another message (y/n): ";
        cin >> doString;
    } while ((doString == "Y") || (doString == "y"));

    return 0;
}
