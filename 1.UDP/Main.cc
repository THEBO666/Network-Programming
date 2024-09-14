#include <fcntl.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <pthread.h>

#include "Test.hpp"
#include "UdpServer.hpp"
void Usage(std::string proc) { std::cout << "\n\rUsage: " << proc << " port[1024+]" << std::endl; }

int main(int argc, char *argv[]) {
    if (argc != 2) {
        Usage(argv[0]);
        exit(0);
    }
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<UdpServer> svr(new UdpServer(port));
    Test *t = new Test();
    t->Translate_Init();
    svr->Init();
    svr->Run([t](const std::string &key) { return t->ExcuteCommand(key); });
    return 0;
}