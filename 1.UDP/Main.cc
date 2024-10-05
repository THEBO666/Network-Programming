#include <fcntl.h>
#include <pthread.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include "Test.hpp"
#include "UdpServer.hpp"
void Usage(std::string proc) { std::cout << "\n\rUsage: " << proc << " port[1024+]" << std::endl; }

int main(int argc, char *argv[]) {
    if (argc != 3) {
        Usage(argv[0]);
        exit(0);
    }
    std::string ip=argv[1];
    uint16_t port = std::stoi(argv[2]);
    std::unique_ptr<UdpServer> svr(new UdpServer(port,ip));
    Test *t = new Test();
    t->Translate_Init();
    svr->Init();
    svr->Run([t](const std::string &key) { return t->ExcuteCommand(key); });
    return 0;
}