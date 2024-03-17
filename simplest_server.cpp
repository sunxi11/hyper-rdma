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
#include <thread>

#define SQ_DEPTH 16

enum ServerState{
    INIT,
    SERVER_BIND,
    REQUEST_GET,
    CONNECTED,
    ROUTE_RESOLVED,
    ADDR_RESOLVED,
};


class simple_server{
public:
    simple_server(const char *ip, int port, void *start_buf, int start_size, void *rdma_buf, int rdma_size){
        this->ip = ip;
        this->port = htobe16(port);
        this->start_buf = (char *)start_buf;
        this->start_size = start_size;
        this->rdma_buf = (char *)rdma_buf;
        this->rdma_size = rdma_size;
    }
    void init();
    void start();
    void rdma_read();
    void rdma_write();
    void bindaddr();
    void cm_thread();
    void cq_thread();
    void setup_buffer();


    private:

    const char *ip;
    __be16 port;
    struct sockaddr_storage *sin;
    char *start_buf;
    int start_size;
    char *rdma_buf;
    int rdma_size;
    enum ServerState state = INIT;

    struct rdma_event_channel *cm_channel;
    struct rdma_cm_id *cm_id;	/* connection on client side,*/
    struct rdma_cm_id *child_cm_id;

    struct ibv_comp_channel *channel;
    struct ibv_cq *cq;
    struct ibv_pd *pd;
    struct ibv_qp *qp;

    struct ibv_recv_wr rq_wr;	/* recv work request record */
    struct ibv_sge recv_sgl;	/* recv single SGE */
    struct rdma_info recv_buf;/* malloc'd buffer */
    struct ibv_mr *recv_mr;		/* MR associated with this buffer */

    struct ibv_send_wr sq_wr;	/* send work request record */
    struct ibv_sge send_sgl;
    struct rdma_info send_buf;/* single send buf */
    struct ibv_mr *send_mr;        /* MR associated with this buffer */

    struct ibv_send_wr rdma_sq_wr;	/* rdma work request record */
    struct ibv_sge rdma_sgl;	/* rdma single SGE */
    struct ibv_mr *rdma_mr;

    struct ibv_mr *start_mr;
};

void simple_server::setup_buffer() {
    int ret;
    recv_mr = ibv_reg_mr(pd, &recv_buf, sizeof(struct rdma_info),
                         IBV_ACCESS_LOCAL_WRITE);
    if (!this->recv_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }

    send_mr = ibv_reg_mr(pd, &send_buf, sizeof(struct rdma_info), 0);
    if (!this->send_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }


    //这里的rdma buf 的内存应该提前分配好
    rdma_mr = ibv_reg_mr(pd, rdma_buf, rdma_size,
                         IBV_ACCESS_LOCAL_WRITE |
                         IBV_ACCESS_REMOTE_READ |
                         IBV_ACCESS_REMOTE_WRITE);
    if (!rdma_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }

    start_mr = ibv_reg_mr(pd, start_buf, start_size,
                          IBV_ACCESS_LOCAL_WRITE |
                          IBV_ACCESS_REMOTE_READ |
                          IBV_ACCESS_REMOTE_WRITE);
    if (!start_mr){
        std::cerr << "ibv_reg_mr error" << std::endl;
        exit(1);
    }


    recv_sgl.addr = (uint64_t )(unsigned long) &this->recv_buf;
    recv_sgl.length = sizeof(struct rdma_info);
    recv_sgl.lkey = this->recv_mr->lkey;
    rq_wr.num_sge = 1;
    rq_wr.sg_list = &this->recv_sgl;


    send_sgl.addr = (uint64_t )(unsigned long) &this->send_buf;
    send_sgl.length = sizeof(struct rdma_info);
    send_sgl.lkey = this->send_mr->lkey;
    sq_wr.num_sge = 1;
    sq_wr.sg_list = &this->send_sgl;
    sq_wr.send_flags = IBV_SEND_SIGNALED;
    sq_wr.opcode = IBV_WR_SEND;


    rdma_sgl.addr = (uint64_t )(unsigned long) this->rdma_buf;
    rdma_sgl.lkey = this->rdma_mr->lkey;
    rdma_sq_wr.num_sge = 1;
    rdma_sq_wr.sg_list = &this->rdma_sgl;
    rdma_sq_wr.send_flags = IBV_SEND_SIGNALED;
}

void simple_server::cq_thread() {
    struct ibv_cq *ev_cq;
    void *ev_ctx;
    int ret;

    while (1){
        ret = ibv_get_cq_event(channel, &ev_cq, &ev_ctx);
        if(ret){
            std::cout << "ibv_get_cq_event error" << std::endl;
            exit(1);
        }
        if(ev_cq != cq){
            std::cout << "ibv_get_cq_event error" << std::endl;
            exit(1);
        }

        ret = ibv_req_notify_cq(cq, 0);
        if(ret){
            std::cout << "ibv_req_notify_cq error" << std::endl;
            exit(1);
        }

        struct ibv_wc wc;
        while ((ret = ibv_poll_cq(cq, 1, &wc)) == 1){
            ret = 0;
            switch (wc.opcode) {
                case IBV_WC_RECV:
                    std::cout << "rdma recv success" << std::endl;
                    break;
                case IBV_WC_SEND:
                    std::cout << "rdma send success" << std::endl;
                default:
                    break;
            }
        }
        ibv_ack_cq_events(cq, 1);
    }
}

void simple_server::cm_thread(){
    struct rdma_cm_event *event;
    struct rdma_cm_id *id;
    while (true){
        int ret = rdma_get_cm_event(cm_channel, &event);
        if (ret) {
            std::cerr << "rdma_get_cm_event error: " << strerror(errno) << std::endl;
            exit(1);
        }
        id = event->id;
        switch (event->event) {
            case RDMA_CM_EVENT_ADDR_RESOLVED:
                ret = rdma_resolve_route(id, 2000);
                if (ret){
                    std::cerr << "rdma_resolve_route error: " << strerror(errno) << std::endl;
                    exit(1);
                }
                state = ADDR_RESOLVED;
                break;
            case RDMA_CM_EVENT_ROUTE_RESOLVED:
                if (state != ADDR_RESOLVED){
                    std::cout << "error" << std::endl;
                }
                state = ROUTE_RESOLVED;
                break;
            case RDMA_CM_EVENT_CONNECT_REQUEST:
                std::cout << "服务端接收到连接请求" << std::endl;
                state = REQUEST_GET;
                child_cm_id = id;
                break;
            case RDMA_CM_EVENT_CONNECT_RESPONSE:
                state = CONNECTED;
                break;

            case RDMA_CM_EVENT_ESTABLISHED:
                //等待服务器接受到地址信息后发送自己的地址信息
//                while(server_message_type != SERVER_SEND_ADDRINFO){}
//                //TODO 交换地址信息
//                send_buf.buf = htobe64((uint64_t)(unsigned long)this->start_buf);
//                send_buf.rkey = htobe32(this->start_mr->rkey);
//                send_buf.size = htobe32(this->start_size);
//                sq_wr.opcode = IBV_WR_SEND_WITH_IMM;
//                sq_wr.imm_data = htobe32(CLIENT_SEND_ADDRINFO);
//
//                ret = ibv_post_send(this->qp, &this->sq_wr, NULL);
//                if (ret) {
//                    std::cerr << "ibv_post_send error: " << strerror(errno) << std::endl;
//                    exit(1);
//                }
                state = CONNECTED;
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

void simple_server::bindaddr() {
    int ret;
    struct addrinfo *res;
    sin = (struct sockaddr_storage*)malloc(sizeof(struct sockaddr_storage));
    if(!sin){
        std::cout << "malloc error" << std::endl;
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

    ret = rdma_bind_addr(cm_id, (struct sockaddr *)this->sin);
    if (ret){
        std::cerr << "rdma_bind_addr error: " << strerror(errno) << std::endl;
        exit(1);
    }

    ret = rdma_listen(cm_id, 3);
    if (ret){
        std::cerr << "rdma_listen error: " << strerror(errno) << std::endl;
        exit(1);
    }

    //等待接收到连接请求

    while (state != REQUEST_GET) {}
}

void simple_server::start() {
    int ret;
    struct ibv_recv_wr *bad_wr;

    cm_channel = create_first_event_channel();
    if(!cm_channel){
        std::cout << "create cm channel error" << std::endl;
        exit(1);
    }

    ret = rdma_create_id(cm_channel, &cm_id, NULL, RDMA_PS_TCP);
    if (ret){
        std::cerr << "rdma_create_id error: " << strerror(errno) << std::endl;
        exit(1);
    }

    //创建监听cm
    std::thread cmthread([this](){
        this->cm_thread();
    });
    cmthread.detach();

    bindaddr();

    pd = ibv_alloc_pd(child_cm_id->verbs);
    if (!pd){
        std::cerr << "ibv_alloc_pd error: " << strerror(errno) << std::endl;
        exit(1);
    }

    channel = ibv_create_comp_channel(child_cm_id->verbs);
    if (!channel){
        std::cerr << "ibv_create_comp_channel error: " << strerror(errno) << std::endl;
        exit(1);
    }

    cq = ibv_create_cq(child_cm_id->verbs, SQ_DEPTH * 2, NULL, channel, 0);
    if (!cq){
        std::cerr << "ibv_create_cq error: " << strerror(errno) << std::endl;
        exit(1);
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

    ret = rdma_create_qp(child_cm_id, pd, &init_attr);
    if(!ret){
        qp = child_cm_id->qp;
    } else{
        std::cerr << "error" << strerror(errno) << std::endl;
        exit(1);
    }
    setup_buffer();

    ret = ibv_post_recv(qp, &rq_wr, &bad_wr);
    if (ret){
        std::cerr << "ibv_post_recv error: " << strerror(errno) << std::endl;
        exit(1);
    }

    //启动cq
    std::thread cqthread([this](){
        this->cq_thread();
    });

    cqthread.detach();

    ret = rdma_accept(child_cm_id, NULL);
    if (ret){
        std::cerr << "rdma_accept error: " << strerror(errno) << std::endl;
        exit(1);
    }

    while (state != CONNECTED){}
    std::cout << "连接建立" << std::endl;
}


int main() {
    char *start_buf = (char *)malloc(32);
    char *rdma_buf = (char *)malloc(32);
    simple_server *server = new simple_server("10.0.0.5", 1245, start_buf, 32, rdma_buf, 32);
    server->start();
};
