/*
 * this.FrankZhou
 * 整数集合接口定义 用于实现set数据类型 不实现升级操作
 */
#ifndef TINY_KV_DB_INT_SET_H
#define TINY_KV_DB_INT_SET_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <algorithm>

class intSet {
private:
    int *contents;
    int currentLength;
public:
    intSet();
    ~intSet();
    // 将给定元素添加到整数集合中 需要保证有序
    void intSetAdd(const int &value);
    // 从整数集合中移除给定元素
    void intSetRemove(const int &value);
    // 查找给定值是否在集合中  整数集合底层有序，使用二分查找，不存在返回-1 存在返回插入位置下标
    int intSetFind(const int &value) const;
    // 从整数集合中随机返回一个元素
    int intSetRandom() const;
    // 根据索引取出整数集合中的元素
    int intSetGet(const int &index) const;
    // 返回整数集合包含的元素个数
    int intSetLen() const;
    // 返回整数集合占用的内存字节数
    int intSetByteLen() const;
};

intSet::intSet() {

}

intSet::~intSet() {

}

void intSet::intSetAdd(const int &value) {

}

void intSet::intSetRemove(const int &value) {

}

int intSet::intSetFind(const int &value) const {

}

int intSet::intSetRandom() const {

}

int intSet::intSetGet(const int &index) const {

}

int intSet::intSetLen() const {

}

int intSet::intSetByteLen() const {

}


#endif //TINY_KV_DB_INT_SET_H
