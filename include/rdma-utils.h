//
// Created by SunX on 2024/3/12.
//

#ifndef HYPER_RDMA_RDMA_UTILS_H
#define HYPER_RDMA_RDMA_UTILS_H

#include <iostream>
#include "rdma_cma.h"
#include "verbs.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdint>

#define SQ_DEPTH 16
enum MESSAGE_TYPE:uint32_t {
    SERVER_SEND_ADDRINFO = 1,
    CLIENT_SEND_ADDRINFO,
    SERVER_READ_COMPLETE,
    SERVER_WRITE_COMPLETE,
    CLIENT_READ_COMPLETE,
    CLIENT_WRITE_COMPLETE

};


struct rdma_info{
    __be64 buf;
    __be32 rkey;
    __be32 size;
};
//send buffer 和 recv buffer 初始化的过程分配，read buffer 和 write buffer 用户提供
class RDMAServer {
public:
    RDMAServer(const char *ip, int port, void *start_buf, int start_size, void *rdma_buf, int rdma_size){
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

private:
    const char *ip;
    __be16 port;
    struct sockaddr_storage *sin;

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
    char *rdma_buf;			/* used as rdma sink */
    int rdma_size;
    struct ibv_mr *rdma_mr;

    uint32_t remote_rkey;		/* remote guys RKEY */
    uint64_t remote_addr;		/* remote guys TO */
    uint32_t remote_len;		/* remote guys LEN */

    char *start_buf;		/* rdma read src */
    int start_size;
    struct ibv_mr *start_mr;

    struct rdma_event_channel *cm_channel;
    struct rdma_cm_id *cm_id;	/* connection on client side,*/
    /* listener on service side. */
    struct rdma_cm_id *child_cm_id;	/* connection on server side */
    enum MESSAGE_TYPE server_message_type;



    bool CONNECTED = false;


    void bindaddr();
    void rdma_buffer_init();
    void handleCmConnections();
    void handleCq();
};


class RDMAclient{
    public:
    RDMAclient(const char *ip, int port, void *start_buf, int start_size, void *rdma_buf, int rdma_size){
        this->ip = ip;
        this->port = htobe16(port);
        this->start_buf = (char *)start_buf;
        this->start_size = start_size;
        this->rdma_buf = (char *)rdma_buf;
        this->rdma_size = rdma_size;
//        this->sin = new sockaddr_storage;
    }
    void init();
    void start();
    void rdma_read();
    void rdma_write();
private :
    const char *ip;
    __be16 port;
    struct sockaddr_storage *sin;

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
    char *rdma_buf;			/* used as rdma sink */
    int rdma_size;
    struct ibv_mr *rdma_mr;

    uint32_t remote_rkey;		/* remote guys RKEY */
    uint64_t remote_addr;		/* remote guys TO */
    uint32_t remote_len;		/* remote guys LEN */

    char *start_buf;		/* rdma read src */
    int start_size;
    struct ibv_mr *start_mr;

    struct rdma_event_channel *cm_channel;
    struct rdma_cm_id *cm_id;	/* connection on client side,*/
    /* listener on service side. */
    struct rdma_cm_id *child_cm_id;	/* connection on server side */
    enum MESSAGE_TYPE server_message_type;



    bool CONNECTED = false;


    void bindaddr();
    void rdma_buffer_init();
    void handleCmConnections();
    void handleCq();
};


#endif //HYPER_RDMA_RDMA_UTILS_H
