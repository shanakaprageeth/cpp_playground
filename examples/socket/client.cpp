// Copyright 2019 Shanaka Prageeth
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#define PORT 8080

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
    int socket_fd = 0;
    struct sockaddr_in serv_addr;
    bool error_occurred = false;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        cout << "socket error" << endl;
        error_occurred = true;
    } else {
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        int inet_status = inet_pton(AF_INET, "127.0.0.1",
            &serv_addr.sin_addr);
        if (inet_status <= 0) {
            cout << "server error" << endl;
            error_occurred = true;
        } else {
            int connect_status = connect(socket_fd,
                (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            if (connect_status < 0) {
                cout << "connection error" << endl;
                error_occurred = true;
            } else {
                string sendString;
                string doString;
                do {
                    char buffer[1024] = {0}; // Moved buffer here
                    cout << "Send: " << endl;
                    cin >> sendString;
                    char *sendMsg = reinterpret_cast<char*>(
                        const_cast<char*>(sendString.c_str()));
                    send(socket_fd, sendMsg, strlen(sendMsg), 0);
                    read(socket_fd, buffer, 1024);
                    cout << "received: " << buffer << endl;
                    cout << "Send another message (y/n): ";
                    cin >> doString;
                } while ((doString == "Y") || (doString == "y"));
            }
        }
    }

    close(socket_fd);
    return error_occurred ? 1 : 0;
}
