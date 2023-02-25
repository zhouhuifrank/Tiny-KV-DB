/*
 * 通用返回对象
 */
#ifndef TINY_KV_DB_CODISOBJECT_H
#define TINY_KV_DB_CODISOBJECT_H

#include <iostream>

template <typename T>
class redisObject {
public:
    std::string type; // 对象类型 string、list、hash、set、sorted set
    unsigned int lruBits; // 记录最近一次访问时间

    int referenceCount; // 引用计数，计数为0的时候进行垃圾回收
    T data;
};

#endif //TINY_KV_DB_CODISOBJECT_H
