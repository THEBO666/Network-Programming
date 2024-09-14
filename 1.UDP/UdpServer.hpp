#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>

#include "Log.hpp"
using func_t = std::function<std::string(const std::string &)>;

enum {
    SOCKET_ERR = 1,
    BIND_ERR,

};

const std::string defaultip = "192.168.3.97";
const uint16_t defaultport = 8080;
const int size = 1024;
class UdpServer {
public:
    UdpServer(const uint16_t &port = defaultport, const std::string &ip = defaultip)
        : sockfd_(0), port_(port), ip_(ip), isrunning_(false) {}
    bool Init() {
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd_ < 0) {
            lg(Fatal, "socket create error, sockfd: %d", sockfd_);
            exit(SOCKET_ERR);
        }
        lg(Info, "socket create sucess, sockfd: %d", sockfd_);
        struct sockaddr_in local;

        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        // local.sin_addr.s_addr = inet_addr(ip_.c_str());
        local.sin_port = htons(port_);
        local.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd_, (const struct sockaddr *)&local, sizeof(local)) < 0) {
            lg(Fatal, "bind error, errno: %d, err string: %s", errno, strerror(errno));
            exit(BIND_ERR);
        }
        lg(Info, "bind sucess, errno: %d, err string: %s", errno, strerror(errno));
        return true;
    }
    void Run(func_t func) {
        isrunning_ = true;
        // 无法使用read write，其是面向字节流的
        char inbuffer[1024];
        while (isrunning_) {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            ssize_t n = recvfrom(sockfd_, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr *)&client, &len);
            if (n < 0) {
                lg(Warning, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }
            inbuffer[n] = 0;
            std::string info = inbuffer;
            std::cout<<"client send# "<<inbuffer<<std::endl;
            // std::string echo_string = "service receive# " + info;
            std::string echo_string = func(info);
            //std::cout<<echo_string<<std::endl;
            sendto(sockfd_, echo_string.c_str(), echo_string.size(), 0, (struct sockaddr *)&client, len);
        }
    }
    ~UdpServer() {
        if (sockfd_ > 0) close(sockfd_);
    }

private:
    int sockfd_;
    uint16_t port_;
    std::string ip_;
    bool isrunning_;
};