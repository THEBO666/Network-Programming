#pragma once

#include <iostream>
#include <sys/socket.h>
#include "Log.hpp"
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
enum
{
    SocketError = 2,
    BindError

};

const int defaultid = -1;
const std::string defaultip="0.0.0.0";
class TcpServer
{
public:
    TcpServer(const uint16_t &port,const std::string &ip=defaultip) : sockfd_(defaultid),port_(port),ip_(ip)
    {
    }
    void Init()
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ < 0)
        {
            lg(Fatal, "create socket error,errno: %d, errstring: %s", errno, strerror(errno));
            exit(SocketError);
        }
        lg(Info,"create socket sucess, sockfd: %d",sockfd_);
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family=AF_INET;
        local.sin_port=htons(port_);
        inet_aton(ip_.c_str(),&(local.sin_addr));//inaddr_any
        if(bind(sockfd_,(struct sockaddr*)&local,sizeof(local))<0)
        {
            lg(Fatal,"bind error. errno: %d, errstring: %s",errno,strerror(errno));
            exit(BindError);
        }



    }
    void Start()
    {
    }
    ~TcpServer() {}

private:
    int sockfd_;
    uint16_t port_;
    std::string ip_;
};