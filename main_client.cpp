#include <iostream>
#include "./include/rdma-utils.h"
#include <thread>




int main() {


    char *start_buf = new char [32];
    char *rdma_buf = new char [32];

    std::strcpy(start_buf, "hello world form client");


    RDMAServer client("10.0.0.2", 12345,
                      start_buf, 32,
                      rdma_buf, 32);

    std::thread clientThread([&client](){
        client.start();
    });


    //rdma 操作之前
    clientThread.join();
    client.rdma_write();
    client.rdma_read();










    std::cout << "Hello, World!" << std::endl;
    return 0;
}
