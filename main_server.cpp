#include <iostream>
#include "./include/rdma-utils.h"
#include <thread>




int main() {


//    char *start_buf = new char [32];
    char *rdma_buf = new char [32];
    char *start_buf = new char [32];

    std::strcpy(start_buf, "hello world form server");

    RDMAServer server("10.0.0.2", 12345,
                      start_buf, 32,
                      rdma_buf, 32);

    std::thread serverThread([&server](){
        server.start();
    });
    serverThread.join();

    server.rdma_write();
    server.rdma_read();


    std::cout << "Hello, World!" << std::endl;
    return 0;
}
