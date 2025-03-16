// Copyright 2019 Shanaka Prageeth

#ifndef INCLUDE_SOCKET_SOCKETSERVER_HPP_
#define INCLUDE_SOCKET_SOCKETSERVER_HPP_

#include <netinet/in.h>  // C system headers
#include <vector>        // C++ system headers
#include <string>        // C++ system headers
#include <functional>    // For std::function
#include <mutex>         // For std::mutex

class SocketServer {
 public:
    SocketServer();
    virtual int initSocket();
    virtual int bindAndListen();
    virtual int acceptClients();
    virtual ~SocketServer() = default;

    void configureSocket(int type, int port_or_path, const std::string &path);
    void handleClient(int client_socket);

    // New method declarations
    void setDataCallback(const std::function<std::string(const std::string&)>& callback);
    void sendDataToClients(const std::vector<int>& clientIds, const std::string& data);

 protected:
    int server_fd;  // File descriptor for the server socket
    int socket_set;  // Result of setsockopt
    int opt = 1;  // Option for setsockopt
    struct sockaddr_in address;  // Server address
    int addrlen = sizeof(address);  // Address length
    int new_socket;  // File descriptor for the accepted socket
    std::vector<int> accepted_sockets;  // List of accepted sockets

    // New member variable
    std::mutex clientMutex;  // Mutex for synchronizing client operations

 private:
    int socket_type;               // Type of socket (e.g., SOCK_STREAM)
    int port;                      // Port number for TCP/UDP sockets
    std::string socket_path;       // Path for UNIX domain sockets
    std::function<std::string(const std::string&)> dataCallback;  // Callback for processing data
};

#endif  // INCLUDE_SOCKET_SOCKETSERVER_HPP_
