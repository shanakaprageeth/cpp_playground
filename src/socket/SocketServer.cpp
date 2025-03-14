// Copyright 2019 Shanaka Prageeth

#include "socket/SocketServer.hpp"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstdio>
#include <string>

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
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return -1;
    }
    return 0;
}

int SocketServer::setSocket() {
    socket_set = setsockopt(server_fd,
                            SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                            &opt, sizeof(opt));
    if (socket_set) {
        perror("Setsockopt failed");
        return -1;
    }
    return 0;
}

int SocketServer::bindAndListen() {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd,
            (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return -1;
    }
    return 0;
}

int SocketServer::acceptClients() {
    new_socket = accept(server_fd, (struct sockaddr *)&address,
                        reinterpret_cast<socklen_t *>(&addrlen));
    if (new_socket < 0) {
        perror("Accept failed");
        return -1;
    }
    accepted_sockets.push_back(new_socket);
    return new_socket;
}

int SocketServer::readAndSend(const char *rcvMsg, const char *sendMsg) {
    char buffer[1024] = {0};
    int valread = read(new_socket, buffer, 1024);
    if (valread < 0) {
        perror("Read failed");
        return -1;
    }
    cout << "Received: " << buffer << endl;

    if (send(new_socket, sendMsg, strlen(sendMsg), 0) < 0) {
        perror("Send failed");
        return -1;
    }
    return 0;
}
