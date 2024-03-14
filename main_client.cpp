#include <iostream>
#include "rdma-utils.h"
#include <thread>
#include <string.h>




int main() {


    char *start_buf = new char [32];
    char *rdma_buf = new char [32];

    strcpy(start_buf, "hello world form client");


    auto *client = new RDMAclient("10.0.0.5", 1245,
                      start_buf, 32,
                      rdma_buf, 32);


    std::thread clientThread([client](){
        client->start();
    });


    //rdma 操作之前
    clientThread.join();
    client->rdma_write();
    client->rdma_read();


    std::cout << "Hello, World!" << std::endl;
    return 0;
}
