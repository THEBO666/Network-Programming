#pragma once

#include <iostream>
#include <sys/socket.h>
#include "Log.hpp"
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <sys/queue.h>

enum
{
    UsageError = 1,
    SocketError = 2,
    BindError,
    ListenError

};

const int backlog = 5;
const int defaultid = -1;
const std::string defaultip = "0.0.0.0";
class TcpServer
{
public:
    TcpServer(const uint16_t &port, const std::string &ip = defaultip) : listensock_(defaultid), port_(port), ip_(ip)
    {
    }
    void Init()
    {
        listensock_ = socket(AF_INET, SOCK_STREAM, 0);
        if (listensock_ < 0)
        {
            lg(Fatal, "create socket error,errno: %d, errstring: %s", errno, strerror(errno));
            exit(SocketError);
        }
        lg(Info, "create socket sucess, sockfd: %d", listensock_);
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);
        // inet_pton(AF_INET, ip_.c_str(), &(local.sin_addr)); // inaddr_any
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(listensock_, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            lg(Fatal, "bind error, errno: %d, errstring: %s", errno, strerror(errno));
            exit(BindError);
        }
        lg(Info, "bind socket sucess, sockfd: %d", listensock_);
        // TCP面向连接的
        if (listen(listensock_, backlog) < 0)
        {
            lg(Fatal, "listen error, errno: %d, errstring: %s", errno, strerror(errno));
            exit(ListenError);
        }
        lg(Info, "listen socket sucess, sockfd: %d", listensock_);
    }
    void Start()
    {
        lg(Info, "tcpserver is connecting...");
        while (true)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int sockfd = accept(listensock_, (struct sockaddr *)&client, &len);
            if (sockfd < 0)
            {
                lg(Warning, "accept error, errno: %d, errstring: %s", errno, strerror(errno));
                continue;
            }
            uint16_t clientport = ntohs(client.sin_port);
            char clientip[32];
            inet_ntop(AF_INET, &(client.sin_addr), clientip, sizeof(clientip));
            lg(Info, "get a new link..., sockfd: %d, client ip: %s, client port: %d", sockfd, clientip, clientport);
            // send(sockfd, "hello\r\n", 8, 0);
            Service(sockfd,clientip,clientport);
        }
    }
    void Service(int sockfd,const std::string&clientip,const uint16_t &clientport)
    {
        char buffer[4096];
        while (true)
        {
            ssize_t n =read(sockfd,buffer,sizeof(buffer));
            if(n>0)
            {
                buffer[n]=0;
                std::cout<<"client say# "<<buffer<<std::endl;
                std::string echo_string ="tcpserver echo# ";
                echo_string+=buffer;
                write(sockfd,echo_string.c_str(),echo_string.size());
            }
        }
        
        
    }
    ~TcpServer()
    {
    }

private:
    int listensock_;
    uint16_t port_;
    std::string ip_;
};