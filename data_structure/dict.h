/*
 * this.FrankZhou
 * 字典-hash表接口定义 实现hash数据类型
 */
#ifndef TINY_KV_DB_DICT_H
#define TINY_KV_DB_DICT_H

#define HASH_INCREMENT "0x3f3f3f3f"

// 斐波那契散列 (i*HASH_INCR+HASH_INCR)&(len-1)
template <typename K,typename V>
class dictEntry {
public:
    K key;
    V value;
    // 使用拉链法解决冲突
    dictEntry* next;
};

template <typename K,typename V>
class dictHash {
public:
    // 哈希表数组
    dictEntry<K,V>** table;
    // 哈希表大小
    unsigned long length;
    // 哈希表大小掩码，用于计算hash Index
    unsigned  long sizeMask;
    // 已经使用节点数量
    unsigned long used;
};

template <typename K,typename V>
class dict {
public:
    // hash函数类型
    int type;
    // 存放两个哈希表 一个用来存放数据 另外一个用来rehash
    dictEntry<K,V> ht[2];
    // rehash进度
    long hashIndex;
    // rehash状态 0暂停 1继续
    int rehashState;
};

#endif //TINY_KV_DB_DICT_H
