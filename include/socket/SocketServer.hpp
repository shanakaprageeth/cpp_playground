// Copyright 2019 Shanaka Prageeth

#ifndef INCLUDE_SOCKET_SOCKETSERVER_HPP_
#define INCLUDE_SOCKET_SOCKETSERVER_HPP_

#include <netinet/in.h>  // C system headers
#include <vector>        // C++ system headers
#include <string>        // C++ system headers

class SocketServer {
 public:  // Adjust indentation
    SocketServer();
    virtual int initSocket();
    virtual int setSocket();
    virtual int bindAndListen();
    virtual int acceptClients();
    virtual int readAndSend(const char *rcvMsg,
        const char *sendMsg);  // Ensure const correctness
    virtual ~SocketServer() = default;

 protected:  // Adjust indentation
    int server_fd;  // File descriptor for the server socket
    int socket_set;  // Result of setsockopt
    int opt = 1;  // Option for setsockopt
    struct sockaddr_in address;  // Server address
    int addrlen = sizeof(address);  // Address length
    int new_socket;  // File descriptor for the accepted socket
    std::vector<int> accepted_sockets;  // List of accepted sockets
};

#endif  // INCLUDE_SOCKET_SOCKETSERVER_HPP_
