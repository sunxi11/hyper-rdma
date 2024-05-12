//
// Created by SunX on 2024/3/19.
//
#include "include/rdma_cma.h"
#include "include/verbs.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>
#include <unistd.h>
#include "include/common.h"
#include <sys/mman.h>  // 包含 mmap() 和 munmap()
#include <fcntl.h>     // 包含 shm_open() 和 O_* 常量
#include <sys/stat.h>  // 包含模式常量
#include <unistd.h>    // 包含 ftruncate() 和 close()



int main(int argc, char *argv[]){
    const char *ip;
    int num_devices;
    struct sockaddr_storage sin;
    struct addrinfo *res;
    struct rdma_cm_id *id;
    struct rdma_event_channel *channel;
    struct ibv_device *dev;
    struct ibv_device_attr dev_attr;
    struct ibv_pd *pd_list[10];
    int ret;

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <ip>" << std::endl;
        return 1;
    }

    ip = argv[1];

    ret = getaddrinfo(ip, NULL, NULL, &res);
    if (ret) {
        std::cout << "Failed to resolve " << ip << std::endl;
        return 1;
    }

    if (res->ai_family != AF_INET) {
        std::cout << "Not an IPv4 address" << std::endl;
        return 1;
    }

    memcpy(&sin, res->ai_addr, sizeof(struct sockaddr_in));
    freeaddrinfo(res);

    channel = create_first_event_channel();
    if (!channel) {
        std::cout << "Failed to create event channel" << std::endl;
        return 1;
    }

    ret = rdma_create_id(channel, &id, NULL, RDMA_PS_TCP);
    if (ret) {
        std::cout << "Failed to create RDMA CM ID" << std::endl;
        return 1;
    }

    ret = rdma_bind_addr(id, (struct sockaddr *)&sin);
    if (ret) {
        std::cout << "Failed to resolve address" << std::endl;
        return 1;
    }

    ret = ibv_query_device(id->verbs, &dev_attr);
    if (ret) {
        std::cout << "Failed to query device" << std::endl;
        return 1;
    }

    std::cout << "Device: " << id->verbs->device->name << std::endl;
    std::cout << "Max WR: " << dev_attr.max_qp_wr << std::endl;
    std::cout << "Max SGE: " << dev_attr.max_sge << std::endl;



    pd_list[0] = ibv_alloc_pd(id->verbs);
    std::cout << pd_list[0]->handle << std::endl;

    pd_list[1] = ibv_alloc_pd(id->verbs);
    std::cout << pd_list[1]->handle << std::endl;

    pd_list[2] = ibv_alloc_pd(id->verbs);
    std::cout << pd_list[2]->handle << std::endl;

    pd_list[3] = ibv_alloc_pd(id->verbs);
    std::cout << pd_list[3]->handle << std::endl;

    pd_list[4] = ibv_alloc_pd(id->verbs);
    std::cout << pd_list[4]->handle << std::endl;


    const char *memname = "sample_shm";
    const size_t region_size = sysconf(_SC_PAGE_SIZE);
    int fd = shm_open(memname, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    void *ptr = mmap(0, region_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    ret = write(fd, "world", 5);  //
    if(ret == -1){
        perror("write");
        return 1;
    }

    std::cout << "read from shared memory: " << (char *)ptr << std::endl;

    // 读数据从共享内存
//    printf("Data read from shared memory: %d\n", *((char*)ptr));


//    while (1){}
    // 解除映射
    munmap(ptr, region_size);



    // 关闭文件描述符
    close(fd);
    rdma_destroy_id(id);
    rdma_destroy_event_channel(channel);



    return 0;

}
