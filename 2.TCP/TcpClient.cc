#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << "serverip serverport\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    inet_pton(AF_INET, serverip.c_str(), &(server.sin_addr));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
        return 1;
    }
    // 客户端不需显示绑定，操作系统自己调解
    // tcp需要连接
    int n = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    if (n < 0)
    {
        std::cerr << "connect error..." << std::endl;
        return 2;
    }
    std::string message;
    while (true)
    {
        std::cout<<"Please Enter@ ";
        std::getline(std::cin,message);
        write(sockfd,message.c_str(),message.size());
        char buffer[4096];
        int n = read(sockfd,buffer,sizeof(buffer));
        if(n>0)
        {
            buffer[n]=0;
            std::cout<<"server say# "<<buffer<<std::endl;
        }
    }

    return 0;
}