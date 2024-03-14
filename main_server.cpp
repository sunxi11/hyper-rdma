#include <iostream>
#include "rdma-utils.h"
#include <thread>
#include <string.h>




int main() {


//    char *start_buf = new char [32];
    char *rdma_buf = new char [32];
    char *start_buf = new char [32];

    strcpy(start_buf, "hello world form server");






    auto *server = new RDMAServer("10.0.0.5", 1245,
                      start_buf, 32,
                      rdma_buf, 32);

//    std::thread serverThread([server](){
//        server->start();
//    });

    server->start()
//    serverThread.join();



    server->rdma_write();
    server->rdma_read();

    while (1){}


    std::cout << "Hello, World!" << std::endl;
    return 0;
}
