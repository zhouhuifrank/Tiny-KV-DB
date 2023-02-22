/*
 * this.FrankZhou
 * 缓存淘汰算法
 */
#ifndef TINY_KV_DB_LRUCACHE_H
#define TINY_KV_DB_LRUCACHE_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

// 节点类定义 key-value存储，key用来与hash表关联
template <typename T>
class Node {
public:
    T key, value;
    Node* prev;
    Node* next;

    Node(int key,int value,Node* prev=nullptr,Node* next=nullptr);
    Node();
    ~Node();
};

// 双向链表定义
class cacheList {
public:
    virtual bool isEmpty() const = 0;
    virtual int length() const = 0;
    virtual void addFromLast(int key,int value) = 0;
    virtual void removeFromFirst() = 0;
    virtual void remove() = 0;
    virtual ~cacheList() {};
};

class DoubleLinkList: public cacheList {
private:
    Node* head;
    Node* tail;
    int currentLength;
    Node* move(int i) const;
public:
    DoubleLinkList();
    ~DoubleLinkList();
    bool isEmpty() const;
    int length() const;
    void addFromLast(int key,int value);
    void removeFromFirst();
    void remove();
};

// 缓存Cache类定义
class cache {
public:
    virtual int get(int key) const = 0;
    virtual void put(int key,int value) = 0;
    virtual ~cache() {};
};

class LRUCache: cache {
private:
    // 双向链表中的节点编号->节点指针
    std::unordered_map<int,Node*> map;
    DoubleLinkList cache;
    int capacity;
    // 缓存工具函数
    // 将某个key提升为最近使用的key
    void makeRecently(int key);
    // 添加最近使用的元素
    void addRecently(int key,int value);
    // 删除一个key
    void deleteKey(int key);
    // 删除最近未使用的元素
    void removeLeastRecently();
public:
    LRUCache(int capacity);
    ~LRUCache();
    int get(int key) const;
    void put(int key,int value);
};

#endif //TINY_KV_DB_LRUCACHE_H
