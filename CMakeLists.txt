cmake_minimum_required(VERSION 3.16)
project(hyper_rdma)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)


add_library(rdmacm_tools STATIC
        common.c
)

# 指定rdma-core头文件和库文件的搜索路径
set(RDMA_CORE_INCLUDE_DIR "/home/zju/rdma_benchmark/rdma-core-49.0/build/include")
set(RDMA_CORE_LIBRARY_DIR "/home/zju/rdma_benchmark/rdma-core-49.0/build/lib")

include_directories(${RDMA_CORE_INCLUDE_DIR})
link_directories(${RDMA_CORE_LIBRARY_DIR})

add_executable(hyper_rdma_client main_client.cpp rdma-utils.cpp)
target_link_libraries(hyper_rdma_client rdmacm ibverbs rdmacm_tools pthread)

add_executable(hyper_rdma_server main_server.cpp rdma-utils.cpp)
target_link_libraries(hyper_rdma_server rdmacm ibverbs rdmacm_tools pthread)

add_executable(simple_client simplest_client.cpp )
target_link_libraries(simple_client rdmacm ibverbs rdmacm_tools pthread)

