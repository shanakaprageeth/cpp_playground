// Copyright 2019 Shanaka Prageeth

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdio>
#include <iostream>
#include <string>
#include "socket/SocketServer.hpp"

using std::cout;
using std::endl;
using std::string;

#define PORT 8080

SocketServer::SocketServer() {
    server_fd = -1;
    new_socket = -1;
    accepted_sockets.clear();
}

int SocketServer::initSocket() {
    int ret_val = 0;  // Initialize return value
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        ret_val = -1;
        perror("Socket failed");
    }
    return ret_val;
}

int SocketServer::setSocket() {
    int ret_val = 0;  // Initialize return value
    socket_set = setsockopt(server_fd,
                            SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                            &opt, sizeof(opt));
    if (socket_set != 0) {
        ret_val = -1;
        perror("Setsockopt failed");
    }
    return ret_val;
}

int SocketServer::bindAndListen() {
    int ret_val = 0;  // Initialize return value
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd,
            reinterpret_cast<struct sockaddr *>(&address),
            sizeof(address)) < 0) {
        ret_val = -1;
        perror("Bind failed");
    } else if (listen(server_fd, 3) < 0) {
        ret_val = -1;
        perror("Listen failed");
    }
    return ret_val;
}

int SocketServer::acceptClients() {
    int ret_val = 0;  // Initialize return value
    new_socket = accept(server_fd,
                        reinterpret_cast<struct sockaddr *>(&address),
                        reinterpret_cast<socklen_t *>(&addrlen));
    if (new_socket < 0) {
        ret_val = -1;
        perror("Accept failed");
    } else {
        accepted_sockets.push_back(new_socket);
        ret_val = new_socket;
    }
    return ret_val;
}

int SocketServer::readAndSend(const char *rcvMsg, const char *sendMsg) {
    int ret_val = 0;  // Initialize return value
    char buffer[1024] = {0};
    int valread = read(new_socket, buffer, 1024);
    if (valread < 0) {
        ret_val = -1;
        perror("Read failed");
    } else {
        cout << "Received: " << buffer << endl;

        if (send(new_socket, sendMsg, strlen(sendMsg), 0) < 0) {
            ret_val = -1;
            perror("Send failed");
        }
    }
    return ret_val;
}
