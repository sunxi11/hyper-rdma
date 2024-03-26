//
// Created by SunX on 2024/3/12.
//

#ifndef HYPER_RDMA_RDMA_UTILS_H
#define HYPER_RDMA_RDMA_UTILS_H

#include <iostream>
#include "include/rdma_cma.h"
#include "include/verbs.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdint>

#define SQ_DEPTH 16

struct rdma_info{
    __be64 buf;
    __be32 rkey;
    __be32 size;
};





#endif //HYPER_RDMA_RDMA_UTILS_H
