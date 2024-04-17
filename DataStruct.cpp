//
// Created by SunX on 2024/4/16.
//

#include <cstdint>
#include <iostream>



#include "DataStruct.h"

RdmaTest::RdmaTest(uint32_t size, uint32_t a, uint32_t b) {
    this->size = size;
    this->a = a;
    this->b = b;
    this->data_buf = new uint32_t[size];

    for(int i = 0; i < size; i++){
        this->data_buf[i] = i;
    }

}


RdmaTest a(10, 1, 1);
RdmaTest b(114541, 1919, 810);

//int main(){
//
//
//    std::cout << a.size << " " << a.a << " " << a.b << std::endl;
//    std::cout << b.size << " " << b.a << " " << b.b << std::endl;
//    std::cout << a.next->size << " " << a.next->a << " " << a.next->b << std::endl;
//
//    return 0;
//
//}
//

