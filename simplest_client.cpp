//
// Created by SunX on 2024/3/14.
//
#include "rdma-utils.h"
#include <infiniband/verbs.h>
#include <rdma/rdma_cma.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <netdb.h>
#include <thread>
#include <endian.h>
#include "common.h"

#define SQ_DEPTH 16

class simple_client{
    public:
    simple_client(const char *ip, int port, void *start_buf, int start_size, void *rdma_buf, int rdma_size);
    void init();
    void start();
    void rdma_read();
    void rdma_write();
    void bindaddr();

    private:
    const char *ip;
    __be16 port;
    struct sockaddr_storage *sin;
    char *start_buf;
    int start_size;
    char *rdma_buf;
    int rdma_size;

    struct rdma_event_channel *cm_channel;
    struct rdma_cm_id *cm_id;	/* connection on client side,*/

    struct ibv_comp_channel *channel;
    struct ibv_cq *cq;
    struct ibv_pd *pd;
    struct ibv_qp *qp;
};

simple_client::simple_client(const char *ip, int port, void *start_buf, int start_size, void *rdma_buf, int rdma_size) {
    this->ip = ip;
    this->port = htobe16(port);
    this->start_buf = (char *)start_buf;
    this->start_size = start_size;
    this->rdma_buf = (char *)rdma_buf;
    this->rdma_size = rdma_size;
}

void simple_client::bindaddr(){
    struct addrinfo *res;
    int ret;
    this->sin = (struct sockaddr_storage *)malloc(sizeof(struct sockaddr_storage));
    if(!this->sin){
        std::cerr << "malloc error" << std::endl;
        exit(1);
    }
    ret = getaddrinfo(this->ip, NULL, NULL, &res);
    if (ret) {
        std::cerr << "getaddrinfo error: " << gai_strerror(ret) << std::endl;
        exit(1);
    }
    if (res->ai_family == PF_INET){
        memcpy(this->sin, res->ai_addr, sizeof(struct sockaddr_in));
    } else if (res->ai_family == PF_INET6){
        memcpy(this->sin, res->ai_addr, sizeof(struct sockaddr_in6));
    } else {
        std::cerr << "unknown address family" << std::endl;
        exit(1);
    }
    if(this->sin->ss_family == AF_INET){
        ((struct sockaddr_in *)this->sin)->sin_port = this->port;
    } else
        ((struct sockaddr_in6 *)this->sin)->sin6_port = this->port;

    ret = rdma_resolve_addr(this->cm_id, NULL, (struct sockaddr *)this->sin, 2000);
    if (ret) {
        std::cerr << "rdma_reslove_addr error: " << strerror(errno) << std::endl;
        exit(1);
    }

    while (1){
        struct rdma_cm_event *event;
        ret = rdma_get_cm_event(this->cm_channel, &event);
        if (ret) {
            std::cerr << "rdma_get_cm_event error: " << strerror(errno) << std::endl;
            exit(1);
        }
        ret = rdma_ack_cm_event(event);
        if (ret) {
            std::cerr << "rdma_ack_cm_event error: " << strerror(errno) << std::endl;
            exit(1);
        }
        if (event->event == RDMA_CM_EVENT_ADDR_RESOLVED) {
            ret = rdma_resolve_route(this->cm_id, 2000);
            if (ret){
                std::cerr << "rdma_resolve_route error: " << strerror(errno) << std::endl;
                exit(1);
            }
            continue;
        }
        if (event->event == RDMA_CM_EVENT_ROUTE_RESOLVED) {
            break;
        }

    }
}

void simple_client::start() {
    int ret;
    struct rdma_cm_id *id;
    cm_channel = create_first_event_channel();
    ret = rdma_create_id(cm_channel, &cm_id, NULL, RDMA_PS_TCP);
    if (ret){
        std::cerr << "rdma_create_id error: " << strerror(errno) << std::endl;
        exit(1);
    }

    bindaddr();

    pd = ibv_alloc_pd(cm_id->verbs);
    if (!pd) {
        std::cerr << "ibv_alloc_pd error: " << strerror(errno) << std::endl;
        exit(1);
    }
    channel = ibv_create_comp_channel(cm_id->verbs);
    if (!channel) {
        std::cerr << "ibv_create_comp_channel error: " << strerror(errno) << std::endl;
        exit(1);
    }
    cq = ibv_create_cq(cm_id->verbs, 2, NULL, channel, 0);
    if (!cq){
        std::cerr << "ibv_create_cq error: " << strerror(errno) << std::endl;
    }

    ret = ibv_req_notify_cq(cq, 0);
    if (ret){
        std::cerr << "ibv_req_notify_cq error: " << strerror(errno) << std::endl;
        exit(1);
    }

    struct ibv_qp_init_attr init_attr;
    memset(&init_attr, 0, sizeof(init_attr));
    init_attr.cap.max_send_wr = SQ_DEPTH;
    init_attr.cap.max_recv_wr = 2;
    init_attr.cap.max_recv_sge = 1;
    init_attr.cap.max_send_sge = 1;
    init_attr.qp_type = IBV_QPT_RC;
    init_attr.send_cq = cq;
    init_attr.recv_cq = cq;
    id = cm_id;

    ret = rdma_create_qp(id, pd, &init_attr);
    if(!ret){
        qp = id->qp;
    } else{
        std::cerr << "error" << strerror(errno) << std::endl;
        exit(1);
    }


    std::cout << "1111111" << std::endl;


}



int main(){
    char *start_buf = (char *)malloc(32);
    char *rdma_buf = (char *)malloc(32);
    simple_client *client = new simple_client("10.0.0.2", 1245, start_buf, 32, rdma_buf, 32);
    client->start();


}