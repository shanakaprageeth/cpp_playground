#include "socket/SocketServer.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[]) 
{ 
    char buffer[1024] = {0}; 
    char *sendMsg;

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
        sendMsg = (char*)sendString.c_str();
        myserver.readAndSend(buffer, sendMsg);
        cout << "Send another message (y/n): ";
        cin >> doString;
    } while (doString == "Y" || doString == "y");

    return 0; 
}
