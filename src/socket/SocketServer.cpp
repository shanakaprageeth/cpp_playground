// Copyright 2019 Shanaka Prageeth

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h> // Added for sockaddr_un
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <functional>
#include "socket/SocketServer.hpp"

using std::cout;
using std::endl;
using std::string;

#define PORT 8080

SocketServer::SocketServer() {
    server_fd = -1;
    new_socket = -1;
    socket_type = SOCK_STREAM;  // Default to TCP
    port = PORT;                // Default port
    socket_path = "";           // Empty for non-UNIX sockets
    accepted_sockets.clear();
}

std::function<std::string(const std::string&)> dataCallback; // Callback for processing data
std::unordered_map<int, int> clientIdentifiers; // Map to store client identifiers

// Allow setting socket type and configuration
void SocketServer::configureSocket(int type, int port_or_path, const std::string &path) {
    socket_type = type;
    if (socket_type == SOCK_STREAM || socket_type == SOCK_DGRAM) {
        port = port_or_path;
    } else if (socket_type == SOCK_SEQPACKET) {
        socket_path = path;
    }
}

int SocketServer::initSocket() {
    int ret_val = 0;  // Initialize return value
    server_fd = socket((socket_type == SOCK_SEQPACKET) ? AF_UNIX : AF_INET, socket_type, 0);
    if (server_fd == -1) {
        ret_val = -1;
        perror("Socket failed");
    }
    return ret_val;
}

int SocketServer::bindAndListen() {
    int ret_val = 0;  // Initialize return value
    if (socket_type == SOCK_SEQPACKET) {
        struct sockaddr_un unix_address;
        memset(&unix_address, 0, sizeof(unix_address));
        unix_address.sun_family = AF_UNIX;
        strncpy(unix_address.sun_path, socket_path.c_str(), sizeof(unix_address.sun_path) - 1);

        if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&unix_address), sizeof(unix_address)) < 0) {
            ret_val = -1;
            perror("Bind failed");
        }
    } else {
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
            ret_val = -1;
            perror("Bind failed");
        }
    }

    if (ret_val == 0 && listen(server_fd, 3) < 0) {
        ret_val = -1;
        perror("Listen failed");
    }
    return ret_val;
}

// Allow users to set a callback for processing received data
void SocketServer::setDataCallback(const std::function<std::string(const std::string&)>& callback) {
    dataCallback = callback;
}

// Function to send data to specific clients based on their identifiers
void SocketServer::sendDataToClients(const std::vector<int>& clientIds, const std::string& data) {
    std::lock_guard<std::mutex> lock(clientMutex);
    for (const auto& clientId : clientIds) {
        auto it = clientIdentifiers.find(clientId);
        if (it != clientIdentifiers.end()) {
            send(it->second, data.c_str(), data.size(), 0);
        }
    }
}

void SocketServer::handleClient(int client_socket) {
    char buffer[1024] = {0};
    while (true) { // Loop to handle multiple messages
        int bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            std::cout << "Client disconnected or error occurred: " << client_socket << std::endl;
            break; // Exit loop if client disconnects or an error occurs
        }

        std::string receivedData(buffer, bytes_read);
        std::cout << "Received: " << receivedData << std::endl;

        // Process data using the callback if set
        if (dataCallback) {
            std::string responseData = dataCallback(receivedData);
            send(client_socket, responseData.c_str(), responseData.size(), 0);
        } else {
            send(client_socket, buffer, bytes_read, 0); // Echo back if no callback is set
        }
    }

    std::cout << "Closing Client: " << client_socket << std::endl;
    close(client_socket);
    std::lock_guard<std::mutex> lock(clientMutex);
    clientIdentifiers.erase(client_socket); // Remove client from map
}

int SocketServer::acceptClients() {
    while (true) {
        int client_socket = accept(server_fd, reinterpret_cast<struct sockaddr *>(&address), reinterpret_cast<socklen_t *>(&addrlen));
        if (client_socket < 0) {
            perror("Accept failed");
            return -1;
        }
        std::cout << "Accept Client" << client_socket << std::endl;
        // Assign a unique identifier to the client
        std::lock_guard<std::mutex> lock(clientMutex);
        clientIdentifiers[client_socket] = client_socket;

        std::thread client_thread(&SocketServer::handleClient, this, client_socket);
        client_thread.detach();
    }
    return 0;
}
