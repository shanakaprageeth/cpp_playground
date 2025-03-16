// Copyright 2019 Shanaka Prageeth
#include <iostream>
#include <string>
#include "socket/SocketServer.hpp"

// Use specific using declarations instead of using-directive
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

// Entry point of the program: initializes and runs the server
int main() {
    SocketServer myserver;

    // Configure the server with type, port, and path
    myserver.configureSocket(SOCK_STREAM, 8080, ""); // TCP socket on port 8080
    myserver.initSocket();
    myserver.bindAndListen();
    cout << "Server is listening on port 8080..." << endl;

    // Set a data callback to process client messages
    myserver.setDataCallback([](const std::string& clientMessage) -> std::string {
        cout << "Processing client message: " << clientMessage << endl;
        return "Echo: " + clientMessage; // Echo the message back with a prefix
    });

    int clientSocket = myserver.acceptClients();

    return 0;
}
