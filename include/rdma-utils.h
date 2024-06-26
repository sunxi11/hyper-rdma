//
// Created by SunX on 2024/3/12.
//

#ifndef HYPER_RDMA_RDMA_UTILS_H
#define HYPER_RDMA_RDMA_UTILS_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdint>

#include "rdma_cma.h"
#include "verbs.h"
#include "../DataStruct.h"

#define SQ_DEPTH 16

struct rdma_info{
    __be64 buf;
    __be32 rkey;
    __be32 size;
};

enum ClientState{
    INIT,
    ROUTE_RESOLVED,
    ADDR_RESOLVED,
    CONNECTED,
};


struct sketch_rdma_info{
    char *data_buf;
    int data_size; //10M


    struct ibv_send_wr sketch_wr;
    struct ibv_mr *sketch_mr;
    struct ibv_sge *sketch_seg;

    sketch_rdma_info(){
        data_size = 1000;
        data_buf = (char *) malloc(data_size * sizeof(int)); //10Mb
        sketch_seg = (struct ibv_sge *) malloc(sizeof(struct ibv_sge));

    }

    uint32_t remote_rkey;		/* remote guys RKEY */
    uint64_t remote_addr;		/* remote guys TO */
    uint32_t remote_len;		/* remote guys LEN */

};


struct test_class_rdma_info{
    struct ibv_send_wr test_class_wr;
    struct ibv_sge *test_sgl;

    struct ibv_mr *test_class_mr;
    struct ibv_mr *class_buf_mr;
    struct ibv_mr *class_send_mr;

    struct rdma_info class_info;
    struct rdma_info clas_buf_info;

    char *class_recv_data;
    char *class_recv_buf;
    char *class_send_buf; //发送端把数据暂存在这里


    uint32_t remote_rkey;		/* remote guys RKEY */
    uint64_t remote_addr;		/* remote guys TO */
    uint32_t remote_len;		/* remote guys LEN */
};


class simple_client{
public:
    simple_client(const char *ip, int port, void *start_buf, int start_size, void *rdma_buf, int rdma_size);
    void init();
    void start();
    void rdma_read();
    void rdma_write();
    void bindaddr();
    void cm_thread();
    void cq_thread();
    void setup_buffer();
    void recv_handler(struct ibv_wc &wc);
    RdmaTest read_class();
    void ow_read();

    bool GET_RDMA_ADDR = false;
    bool RDMA_READ_COMPLETE = false;
    bool RDMA_WRITE_COMPLETE = false;
    bool GET_CLASS_ADDR = false;
    bool GET_SKETCH_ADDR = false;

private:
    const char *ip;
    __be16 port;
    struct sockaddr_storage *sin;
    char *start_buf;
    int start_size;
    char *rdma_buf;
    int rdma_size;
    enum ClientState state = INIT;


    struct rdma_event_channel *cm_channel;
    struct rdma_cm_id *cm_id;	/* connection on client side,*/

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

    uint32_t remote_rkey;		/* remote guys RKEY */
    uint64_t remote_addr;		/* remote guys TO */
    uint32_t remote_len;		/* remote guys LEN */

    struct test_class_rdma_info class_info;
    struct sketch_rdma_info sketch_info;


};


enum ServerState{
    SERVER_INIT,
    SERVER_BIND,
    REQUEST_GET,
    SERVER_CONNECTED,
    SERVER_READ_ADV,
    SERVER_WRITE_ADV,
    SERVER_WRITE_COMPLETE1,
    SERVER_READ_COMPLETE1,
    SERVER_RDMA_ADDR_SEND_COMPLETE,
    CLASS_ADDR_SEND_COMPLETE,
    SKETCH_ADDR_SEND_COMPLETE,
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
    void server_recv_handler(struct ibv_wc&);
    void init_class_recv();
    void init_sketch_rdma();


private:

    const char *ip;
    __be16 port;
    struct sockaddr_storage *sin;
    char *start_buf;
    int start_size;
    char *rdma_buf;
    int rdma_size;
    enum ServerState state = SERVER_INIT;
    bool SERVER_GET_REMOTE_ADDR = false;

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

    uint32_t remote_rkey;		/* remote guys RKEY */
    uint64_t remote_addr;		/* remote guys TO */
    uint32_t remote_len;		/* remote guys LEN */


    struct test_class_rdma_info class_info;
    struct sketch_rdma_info sketch_info;

    bool rdma_addr_send_complete = false;
    bool class_addr_send_complete = false;
    bool sketch_addr_send_complete = false;


};



#endif //HYPER_RDMA_RDMA_UTILS_H
