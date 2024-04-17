cmake_minimum_required(VERSION 3.16)
project(hyper_rdma)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)


add_library(rdmacm_tools STATIC
        common.c
)

set(RDMA_CORE_INCLUDE_DIR "/root/rdma-core-49.0/build/include")
set(RDMA_CORE_LIBRARY_DIR "/root/rdma-core-49.0/build/lib")

include_directories(${RDMA_CORE_INCLUDE_DIR})

link_directories(${RDMA_CORE_LIBRARY_DIR})

#add_executable(test DataStruct.cpp)


add_executable(simple_client simplest_client.cpp
        DataStruct.cpp
        DataStruct.h)
target_link_libraries(simple_client rdmacm ibverbs rdmacm_tools pthread)

add_executable(simple_server simplest_server.cpp
        DataStruct.cpp
        DataStruct.h)
target_link_libraries(simple_server rdmacm ibverbs rdmacm_tools pthread)

add_executable(ibinfo get_ibinfo.cpp)
target_link_libraries(ibinfo rdmacm ibverbs rdmacm_tools pthread)

