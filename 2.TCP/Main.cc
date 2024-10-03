#include "TcpServer.hpp"
#include <memory>

int main(int argc,char *argv[])
{
    std::unique_ptr<TcpServer> tcp_svr(new TcpServer());
    tcp_svr->Init();
    tcp_svr->Start();
    return 0;
}