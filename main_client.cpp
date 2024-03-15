#include <iostream>
#include "rdma-utils.h"
#include <thread>
#include <string.h>
#include <netdb.h>

void RDMAclient::send_first_infomation(){
    int ret;
    struct ibv_send_wr *bad_wr;

    send_buf.buf = htobe64((uint64_t)(unsigned long)this->start_buf);
    send_buf.rkey = htobe32(this->start_mr->rkey);
    send_buf.size = htobe32(this->start_size);
    sq_wr.opcode = IBV_WR_SEND_WITH_IMM;
    sq_wr.imm_data = htobe32(CLIENT_SEND_ADDRINFO);
    ret = ibv_post_send(this->qp, &this->sq_wr, &bad_wr);
    if(ret){
        std::cout << "post send error" << std::endl;
        exit(1);
    }
}




int main() {


    char *start_buf = new char [32];
    char *rdma_buf = new char [32];

    strcpy(start_buf, "hello world form client");


    auto *client = new RDMAclient("10.0.0.5", 1245,
                      start_buf, 32,
                      rdma_buf, 32);


//    std::thread clientThread([client](){
//        client->start();
//    });

    client->start();
    client->send_first_infomation();




    //rdma 操作之前
//    clientThread.join();

//    client->rdma_write();
//    client->rdma_read();

    while (1){}


    std::cout << "Hello, World!" << std::endl;
    return 0;
}
