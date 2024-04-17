//
// Created by SunX on 2024/4/16.
//

#ifndef HYPER_RDMA_DATASTRUCT_H
#define HYPER_RDMA_DATASTRUCT_H

#include <cstdint>


class RdmaTest{
public:
    uint32_t size;
    uint32_t a;
    uint32_t b;
    uint32_t *data_buf;
    RdmaTest *next;
    RdmaTest(uint32_t size, uint32_t a, uint32_t b);


};

extern RdmaTest a;
extern RdmaTest b;

#endif //HYPER_RDMA_DATASTRUCT_H
