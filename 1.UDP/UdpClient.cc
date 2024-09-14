#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <string>

#include "Log.hpp"

enum {
    SOCKET_ERR = 1,
    BIND_ERR,

};
void Usage(std::string proc) { std::cout << "\n\rUsage: " << proc << " serverip serverport" << std::endl; }
int main(int argc, char *argv[]) {
    if (argc != 3) {
        Usage(argv[0]);
        exit(0);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        lg(Fatal, "socket create error, sockfd: %d", sockfd);
        exit(SOCKET_ERR);
    }
    /*
        客户端不需要显示绑定
    */
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_addr.s_addr = inet_addr(serverip.c_str());
    server.sin_port = ntohs(serverport);
    server.sin_family = AF_INET;
    char buffer[1024];
    std::string message;
    while (1) {
        std::cout << "Please Enter@ ";
        std::getline(std::cin, message);
        sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, sizeof(server));
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        ssize_t n = recvfrom(sockfd, buffer, 1023, 0, (struct sockaddr *)&temp, &len);
        if (n > 0) {
            buffer[n] = 0;
            std::cout << buffer << std::endl;
        }
    }
    return 0;
}