//
// Created by SunX on 2024/3/12.
//
#include "./include/rdma-utils.h"
#include "./include/verbs.h"
#include "./include/rdma_cma.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <netdb.h>
#include <thread>
#include <endian.h>


void RDMAServer::start() {
    RDMAServer::init();
    //初始化完成后等待连接的建立
    std::thread connThread([this](){
        this->handleCmConnections();
    });
    connThread.detach(); //detach 后的线程与创建它的线程生命周期脱钩，成为了一个完全独立的执行流

    //创建一个线程处理cq
    std::thread cqThread([this](){
        this->handleCq();
    });

    while (CONNECTED == false){}
    std::cout << "连接建立" << std::endl;

}

void RDMAServer::handleCq(){
    struct ibv_cq *ev_cq;
    struct ibv_wc *wc;
    int ret;
    int cq_num;
    void *ev_ctx;
    struct ibv_recv_wr *bad_wr;

    while (1){
        ret = ibv_get_cq_event(this->channel, &ev_cq, &ev_ctx);
        if (ret){
            std::cerr << "failed to get cq event" << strerror(errno) << std::endl;
            exit(1);
        }

        if(ev_cq != this->cq){
            std::cerr << "unknown cq" << std::endl;
            exit(1);
        }

        ret = ibv_req_notify_cq(this->cq, 0);
        if (ret){
            std::cerr << "failed to request notify cq" << strerror(errno) << std::endl;
            exit(1);
        }

        cq_num = ibv_poll_cq(this->cq, 1, wc);
        for (int i = 0; i < cq_num; ++i) {
            switch (wc->opcode) {
                case IBV_WC_SEND:
                    std::cout << "rdma send complete!" << std::endl;
                    break;
                case IBV_WC_RECV:
                    //对于客户端
                    if(wc->byte_len != sizeof recv_buf){
                        std::cerr << "recieve buf data" << strerror(errno) << std::endl;
                        exit(1);
                    }
                    remote_addr = be32toh(recv_buf.buf);
                    remote_rkey = be32toh(recv_buf.rkey);
                    remote_len = be32toh(recv_buf.size);
                    ret = ibv_post_recv(qp, &rq_wr, &bad_wr);
                    if (ret){
                        std::cerr << "ibv_post_recv error" << strerror(errno) << std::endl;
                        exit(1);
                    }
                    break;
                case IBV_WC_RDMA_WRITE:
                    std::cout << "rdma write complete!" << std::endl;
                    server_message_type = SERVER_WRITE_COMPLETE;
                    break;
                case IBV_WC_RDMA_READ:
                    std::cout << "rdma read/write complete!" << std::endl;
                    server_message_type = SERVER_READ_COMPLETE;
                    break;
                case IBV_WC_RECV_RDMA_WITH_IMM:
                    server_message_type = be32toh(wc->imm_data);
                    remote_addr = be32toh(recv_buf.buf);
                    remote_rkey = be32toh(recv_buf.rkey);
                    remote_len = be32toh(recv_buf.size);
                    ret = ibv_post_recv(qp, &rq_wr, &bad_wr);
                    if (ret){
                        std::cerr << "ibv_post_recv error" << strerror(errno) << std::endl;
                        exit(1);
                    }
                    break;

                default:
                    std::cerr << "unknown opcode" << std::endl;
                    exit(1);

            }
        }

    }
}

void RDMAServer::rdma_read() {
    struct ibv_send_wr *bad_wr;
    int ret;

    //把远端的地址读到 rdma buf

    rdma_sq_wr.wr.rdma.remote_addr = remote_addr;
    rdma_sq_wr.wr.rdma.rkey = remote_rkey;

    rdma_sgl.addr = (uint64_t )(unsigned long) this->rdma_buf;
    rdma_sgl.lkey = this->rdma_mr->lkey;
    rdma_sgl.length = remote_len;

    rdma_sq_wr.num_sge = 1;
    rdma_sq_wr.sg_list = &this->rdma_sgl;
    rdma_sq_wr.opcode = IBV_WR_RDMA_READ;

    ret = ibv_post_send(this->qp, &rdma_sq_wr, &bad_wr);
    if (ret){
        std::cerr << "ibv_post_send error" << strerror(errno) << std::endl;
        exit(1);
    }

    //等待 read 完成
    while (server_message_type != SERVER_READ_COMPLETE){}
        std::cout << (char *)rdma_buf << std::endl;
}

void RDMAServer::rdma_write() {
    struct ibv_send_wr *bad_wr;
    int ret;

    //把本地的地址写到远端的地址
    rdma_sq_wr.wr.rdma.remote_addr = remote_addr;
    rdma_sq_wr.wr.rdma.rkey = remote_rkey;

    rdma_sgl.addr = (uint64_t )(unsigned long) this->start_buf;
    rdma_sgl.lkey = this->start_mr->lkey;
    rdma_sgl.length = this->start_size;

    rdma_sq_wr.num_sge = 1;
    rdma_sq_wr.sg_list = &rdma_sgl;
    rdma_sq_wr.opcode = IBV_WR_RDMA_WRITE;

    ret = ibv_post_send(this->qp, &rdma_sq_wr, &bad_wr);







}

void RDMAServer::handleCmConnections() {
    struct rdma_cm_event *event;
    while (true){
        int ret = rdma_get_cm_event(this->cm_channel, &event);
        if (ret) {
            std::cerr << "rdma_get_cm_event error: " << strerror(errno) << std::endl;
            exit(1);
        }
        switch (event->event) {
            case RDMA_CM_EVENT_ESTABLISHED:
                //TODO 交换地址信息
                send_buf.buf = htobe64((uint64_t)(unsigned long)this->start_buf);
                send_buf.rkey = htobe32(this->start_mr->rkey);
                send_buf.size = htobe32(this->start_size);
                sq_wr.opcode = IBV_WR_SEND_WITH_IMM;
                sq_wr.imm_data = htobe32(SERVER_SEND_ADDRINFO);

                ret = ibv_post_send(this->qp, &this->sq_wr, NULL);
                if (ret) {
                    std::cerr << "ibv_post_send error: " << strerror(errno) << std::endl;
                    exit(1);
                }
                //等待服务器接受到地址信息后发送自己的地址信息
                while(server_message_type != CLIENT_SEND_ADDRINFO){}
                this->CONNECTED = true;
                break;
            default:
                break;
        }
        ret = rdma_ack_cm_event(event);
        if (ret) {
            std::cerr << "rdma_ack_cm_event error: " << strerror(errno) << std::endl;
            exit(1);
        }
    }

}
//先调用这个函数
void RDMAServer::init() {
    int ret;
    struct ibv_qp_init_attr init_attr = {};
    init_attr.cap.max_send_wr = SQ_DEPTH;
    init_attr.cap.max_recv_wr = 2;
    init_attr.cap.max_recv_sge = 1;
    init_attr.cap.max_send_sge = 1;
    init_attr.qp_type = IBV_QPT_RC;
    init_attr.send_cq = this->cq;
    init_attr.recv_cq = this->cq;

    this->cm_channel = rdma_create_event_channel();
    if (!this->cm_channel) {
        std::cerr << "rdma_create_event_channel error" << std::endl;
        exit(1);
    }
    ret = rdma_create_id(this->cm_channel, &this->cm_id, NULL, RDMA_PS_TCP);
    if (ret) {
        std::cerr << "rdma_create_id error" << std::endl;
        exit(1);
    }

    RDMAServer::bindaddr(); //调用liston并阻塞，直到有连接请求

    this->pd = ibv_alloc_pd(this->cm_id->verbs);
    if (!this->pd) {
        std::cerr << "ibv_alloc_pd error" << std::endl;
        exit(1);
    }

    this->channel = ibv_create_comp_channel(this->cm_id->verbs);
    if (!this->channel) {
        std::cerr << "ibv_create_comp_channel error" << std::endl;
        exit(1);
    }

    this->cq = ibv_create_cq(this->cm_id->verbs, SQ_DEPTH * 2, NULL, this->channel, 0);
    if (!this->cq) {
        std::cerr << "ibv_create_cq error" << std::endl;
        exit(1);
    }

    ret = ibv_req_notify_cq(this->cq, 0);
    if (ret) {
        std::cerr << "ibv_req_notify_cq error" << std::endl;
        exit(1);
    }

    ret = rdma_create_qp(this->cm_id, this->pd, &init_attr);
    if (ret) {
        std::cerr << "rdma_create_qp error" << std::endl;
        exit(1);
    }
    this->qp = this->cm_id->qp;

    RDMAServer::rdma_buffer_init();

    ret = ibv_post_recv(this->qp, &this->rq_wr, NULL);
    if (ret) {
        std::cerr << "ibv_post_recv error" << std::endl;
        exit(1);
    }

    ret = rdma_accept(this->child_cm_id, NULL);
    if (ret) {
        std::cerr << "rdma_accept error" << std::endl;
        exit(1);
    }

}

void RDMAServer::bindaddr() {
    struct addrinfo *res;
    int ret;
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

    ret = rdma_bind_addr(this->cm_id, (struct sockaddr *)this->sin);
    if (ret) {
        std::cerr << "rdma_bind_addr error: " << strerror(errno) << std::endl;
        exit(1);
    }

    ret = rdma_listen(this->cm_id, 10);
    if (ret) {
        std::cerr << "rdma_listen error: " << strerror(errno) << std::endl;
        exit(1);
    }

    //等待连接
    while(1){
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
        if (event->event == RDMA_CM_EVENT_CONNECT_REQUEST) {
            this->child_cm_id = this->cm_id;
            break;
        }
    }

}

void RDMAServer::rdma_buffer_init() {
    int ret;

    this->recv_mr = ibv_reg_mr(this->pd, &this->recv_buf, sizeof(struct rdma_info),
            IBV_ACCESS_LOCAL_WRITE);
    if (!this->recv_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }

    this->send_mr = ibv_reg_mr(this->pd, &this->send_buf, sizeof(struct rdma_info), 0);
    if (!this->send_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }


    //这里的rdma buf 的内存应该提前分配好
    this->rdma_mr = ibv_reg_mr(this->pd, this->rdma_buf, this->rdma_size,
                               IBV_ACCESS_LOCAL_WRITE |
                               IBV_ACCESS_REMOTE_READ |
                               IBV_ACCESS_REMOTE_WRITE);
    if (!this->rdma_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }

    this->start_mr = ibv_reg_mr(this->pd, this->start_buf, this->start_size,
                                IBV_ACCESS_LOCAL_WRITE |
                                IBV_ACCESS_REMOTE_READ |
                                IBV_ACCESS_REMOTE_WRITE);
    if (!this->start_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }


    this->recv_sgl.addr = (uint64_t )(unsigned long) &this->recv_buf;
    this->recv_sgl.length = sizeof(struct rdma_info);
    this->recv_sgl.lkey = this->recv_mr->lkey;
    this->rq_wr.num_sge = 1;
    this->rq_wr.sg_list = &this->recv_sgl;


    this->send_sgl.addr = (uint64_t )(unsigned long) &this->send_buf;
    this->send_sgl.length = sizeof(struct rdma_info);
    this->send_sgl.lkey = this->send_mr->lkey;
    this->sq_wr.num_sge = 1;
    this->sq_wr.sg_list = &this->send_sgl;
    this->sq_wr.send_flags = IBV_SEND_SIGNALED;
    this->sq_wr.opcode = IBV_WR_SEND;


    this->rdma_sgl.addr = (uint64_t )(unsigned long) this->rdma_buf;
    this->rdma_sgl.lkey = this->rdma_mr->lkey;
    this->rdma_sq_wr.num_sge = 1;
    this->rdma_sq_wr.sg_list = &this->rdma_sgl;
    this->rdma_sq_wr.send_flags = IBV_SEND_SIGNALED;
}

void RDMAclient::bindaddr() {
    struct addrinfo *res;
    int ret;
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

void RDMAclient::rdma_buffer_init() {
    int ret;

    this->recv_mr = ibv_reg_mr(this->pd, &this->recv_buf, sizeof(struct rdma_info),
                               IBV_ACCESS_LOCAL_WRITE);
    if (!this->recv_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }

    this->send_mr = ibv_reg_mr(this->pd, &this->send_buf, sizeof(struct rdma_info), 0);
    if (!this->send_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }


    //这里的rdma buf 的内存应该提前分配好
    this->rdma_mr = ibv_reg_mr(this->pd, this->rdma_buf, this->rdma_size,
                               IBV_ACCESS_LOCAL_WRITE |
                               IBV_ACCESS_REMOTE_READ |
                               IBV_ACCESS_REMOTE_WRITE);
    if (!this->rdma_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }

    this->start_mr = ibv_reg_mr(this->pd, this->start_buf, this->start_size,
                                IBV_ACCESS_LOCAL_WRITE |
                                IBV_ACCESS_REMOTE_READ |
                                IBV_ACCESS_REMOTE_WRITE);
    if (!this->start_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }


    this->recv_sgl.addr = (uint64_t )(unsigned long) &this->recv_buf;
    this->recv_sgl.length = sizeof(struct rdma_info);
    this->recv_sgl.lkey = this->recv_mr->lkey;
    this->rq_wr.num_sge = 1;
    this->rq_wr.sg_list = &this->recv_sgl;


    this->send_sgl.addr = (uint64_t )(unsigned long) &this->send_buf;
    this->send_sgl.length = sizeof(struct rdma_info);
    this->send_sgl.lkey = this->send_mr->lkey;
    this->sq_wr.num_sge = 1;
    this->sq_wr.sg_list = &this->send_sgl;
    this->sq_wr.send_flags = IBV_SEND_SIGNALED;
    this->sq_wr.opcode = IBV_WR_SEND;


    this->rdma_sgl.addr = (uint64_t )(unsigned long) this->rdma_buf;
    this->rdma_sgl.lkey = this->rdma_mr->lkey;
    this->rdma_sq_wr.num_sge = 1;
    this->rdma_sq_wr.sg_list = &this->rdma_sgl;
    this->rdma_sq_wr.send_flags = IBV_SEND_SIGNALED;
}

void RDMAclient::init() {
    int ret;
    struct ibv_qp_init_attr init_attr = {};
    init_attr.cap.max_send_wr = SQ_DEPTH;
    init_attr.cap.max_recv_wr = 2;
    init_attr.cap.max_recv_sge = 1;
    init_attr.cap.max_send_sge = 1;
    init_attr.qp_type = IBV_QPT_RC;
    init_attr.send_cq = this->cq;
    init_attr.recv_cq = this->cq;

    this->cm_channel = rdma_create_event_channel();
    if (!this->cm_channel) {
        std::cerr << "rdma_create_event_channel error" << std::endl;
        exit(1);
    }
    ret = rdma_create_id(this->cm_channel, &this->cm_id, NULL, RDMA_PS_TCP);
    if (ret) {
        std::cerr << "rdma_create_id error" << std::endl;
        exit(1);
    }

    RDMAclient::bindaddr(); //

    this->pd = ibv_alloc_pd(this->cm_id->verbs);
    if (!this->pd) {
        std::cerr << "ibv_alloc_pd error" << std::endl;
        exit(1);
    }

    this->channel = ibv_create_comp_channel(this->cm_id->verbs);
    if (!this->channel) {
        std::cerr << "ibv_create_comp_channel error" << std::endl;
        exit(1);
    }

    this->cq = ibv_create_cq(this->cm_id->verbs, SQ_DEPTH * 2, NULL, this->channel, 0);
    if (!this->cq) {
        std::cerr << "ibv_create_cq error" << std::endl;
        exit(1);
    }

    ret = ibv_req_notify_cq(this->cq, 0);
    if (ret) {
        std::cerr << "ibv_req_notify_cq error" << std::endl;
        exit(1);
    }

    ret = rdma_create_qp(this->cm_id, this->pd, &init_attr);
    if (ret) {
        std::cerr << "rdma_create_qp error" << std::endl;
        exit(1);
    }
    this->qp = this->cm_id->qp;

    RDMAclient::rdma_buffer_init();

    ret = ibv_post_recv(this->qp, &this->rq_wr, NULL);
    if (ret) {
        std::cerr << "ibv_post_recv error" << std::endl;
        exit(1);
    }

    struct rdma_conn_param conn_param;
    conn_param.responder_resources = 1;
    conn_param.initiator_depth = 1;
    conn_param.retry_count = 7;
    conn_param.rnr_retry_count = 7;

    ret = rdma_connect(this->cm_id, NULL);
    if (ret) {
        std::cerr << "rdma_accept error" << std::endl;
        exit(1);
    }
}

void RDMAclient::handleCmConnections() {
    struct rdma_cm_event *event;
    while (true){
        int ret = rdma_get_cm_event(this->cm_channel, &event);
        if (ret) {
            std::cerr << "rdma_get_cm_event error: " << strerror(errno) << std::endl;
            exit(1);
        }
        switch (event->event) {
            case RDMA_CM_EVENT_ESTABLISHED:
                //等待服务器接受到地址信息后发送自己的地址信息
                while(server_message_type != SERVER_SEND_ADDRINFO){}
                //TODO 交换地址信息
                send_buf.buf = htobe64((uint64_t)(unsigned long)this->start_buf);
                send_buf.rkey = htobe32(this->start_mr->rkey);
                send_buf.size = htobe32(this->start_size);
                sq_wr.opcode = IBV_WR_SEND_WITH_IMM;
                sq_wr.imm_data = htobe32(CLIENT_SEND_ADDRINFO);

                ret = ibv_post_send(this->qp, &this->sq_wr, NULL);
                if (ret) {
                    std::cerr << "ibv_post_send error: " << strerror(errno) << std::endl;
                    exit(1);
                }
                this->CONNECTED = true;
                break;
            default:
                break;
        }
        ret = rdma_ack_cm_event(event);
        if (ret) {
            std::cerr << "rdma_ack_cm_event error: " << strerror(errno) << std::endl;
            exit(1);
        }
    }


}

void RDMAclient::handleCq() {
    struct ibv_cq *ev_cq;
    struct ibv_wc *wc;
    int ret;
    int cq_num;
    void *ev_ctx;
    struct ibv_recv_wr *bad_wr;

    while (1){
        ret = ibv_get_cq_event(this->channel, &ev_cq, &ev_ctx);
        if (ret){
            std::cerr << "failed to get cq event" << strerror(errno) << std::endl;
            exit(1);
        }
        if(ev_cq != this->cq){
            std::cerr << "unknown cq" << std::endl;
            exit(1);
        }

        ret = ibv_req_notify_cq(this->cq, 0);
        if (ret){
            std::cerr << "failed to request notify cq" << strerror(errno) << std::endl;
            exit(1);
        }

        cq_num = ibv_poll_cq(this->cq, 1, wc);
        for (int i = 0; i < cq_num; ++i) {
            switch (wc->opcode) {
                case IBV_WC_SEND:
                    std::cout << "rdma send complete!" << std::endl;
                    break;
                case IBV_WC_RECV:
                    //客户端
                    if(wc->byte_len != sizeof recv_buf){
                        std::cerr << "recieve buf data" << strerror(errno) << std::endl;
                        exit(1);
                    }
                    remote_addr = be32toh(recv_buf.buf);
                    remote_rkey = be32toh(recv_buf.rkey);
                    remote_len = be32toh(recv_buf.size);
                    ret = ibv_post_recv(qp, &rq_wr, &bad_wr);
                    if (ret){
                        std::cerr << "ibv_post_recv error" << strerror(errno) << std::endl;
                        exit(1);
                    }
                    break;
                case IBV_WC_RDMA_WRITE:
                    std::cout << "rdma write complete!" << std::endl;
                    server_message_type = CLIENT_WRITE_COMPLETE;
                    break;
                case IBV_WC_RDMA_READ:
                    std::cout << "rdma read/write complete!" << std::endl;
                    server_message_type = CLIENT_READ_COMPLETE;
                    break;
                case IBV_WC_RECV_RDMA_WITH_IMM:
                    server_message_type = be32toh(wc->imm_data);
                    remote_addr = be32toh(recv_buf.buf);
                    remote_rkey = be32toh(recv_buf.rkey);
                    remote_len = be32toh(recv_buf.size);
                    ret = ibv_post_recv(qp, &rq_wr, &bad_wr);
                    if (ret){
                        std::cerr << "ibv_post_recv error" << strerror(errno) << std::endl;
                        exit(1);
                    }
                    break;

                default:
                    std::cerr << "unknown opcode" << std::endl;
                    exit(1);

            }
        }
    }
}

void RDMAclient::start() {
    RDMAclient::init();
    //初始化完成后等待连接的建立
    std::thread connThread([this](){
        this->handleCmConnections();
    });
    connThread.detach(); //detach 后的线程与创建它的线程生命周期脱钩，成为了一个完全独立的执行流

    //创建一个线程处理cq
    std::thread cqThread([this](){
        this->handleCq();
    });

    while (CONNECTED == false){}
    std::cout << "连接建立" << std::endl;

}

void RDMAclient::rdma_read() {
    struct ibv_send_wr *bad_wr;
    int ret;

    //把远端的地址读到 rdma buf
    rdma_sgl.addr = (uint64_t )(unsigned long) this->rdma_buf;
    rdma_sgl.lkey = this->rdma_mr->lkey;
    rdma_sgl.length = remote_len;

    rdma_sq_wr.wr.rdma.remote_addr = remote_addr;
    rdma_sq_wr.wr.rdma.rkey = remote_rkey;

    rdma_sq_wr.num_sge = 1;
    rdma_sq_wr.sg_list = &this->rdma_sgl;
    rdma_sq_wr.opcode = IBV_WR_RDMA_READ;

    ret = ibv_post_send(this->qp, &rdma_sq_wr, &bad_wr);
    if (ret){
        std::cerr << "ibv_post_send error" << strerror(errno) << std::endl;
        exit(1);
    }

    //等待 read 完成
    while (server_message_type != CLIENT_READ_COMPLETE){}
    std::cout << (char *)rdma_buf << std::endl;

}

void RDMAclient::rdma_write() {
    struct ibv_send_wr *bad_wr;
    int ret;

    //把本地的地址写到远端的地址
    rdma_sgl.addr = (uint64_t )(unsigned long) this->start_buf;
    rdma_sgl.lkey = this->start_mr->lkey;
    rdma_sgl.length = this->start_size;

    rdma_sq_wr.wr.rdma.remote_addr = remote_addr;
    rdma_sq_wr.wr.rdma.rkey = remote_rkey;

    rdma_sq_wr.num_sge = 1;
    rdma_sq_wr.sg_list = &rdma_sgl;
    rdma_sq_wr.opcode = IBV_WR_RDMA_WRITE;

    ret = ibv_post_send(this->qp, &rdma_sq_wr, &bad_wr);
    if (ret){
        std::cerr << "ibv_post_send error" << strerror(errno) << std::endl;
        exit(1);
    }

    while (server_message_type != CLIENT_WRITE_COMPLETE){}
    std::cout << "write complete!" << std::endl;

}



