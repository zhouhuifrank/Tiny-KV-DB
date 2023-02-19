/*
 * this.FrankZhou
 * 双向链表模板类
 */
#ifndef TINY_KV_DB_LINKED_LIST_H
#define TINY_KV_DB_LINKED_LIST_H

#include <iostream>
#include <cstring>
#include <algorithm>

// 链表节点定义
template <typename E>
class listNode {
private:
    E value;
private:
    listNode* prev;
    listNode* next;
public:
    listNode();
    listNode(const E &value,listNode* prev=nullptr,listNode* next=nullptr);
    ~listNode();
    void setValue(const E &value);
    E getValue() const;
};

template <typename E>
listNode<E>::listNode(const E &value, listNode<E> *prev, listNode<E> *next) {
    this->value = value;
    this->prev = prev;
    this->next = next;
}

template <typename E>
listNode<E>::listNode() {
    this->prev = nullptr;
    this->prev = nullptr;
}

template <typename E>
listNode<E>::~listNode() {
    delete prev;
    delete next;
}

template <typename E>
void listNode<E>::setValue(const E &value) {
    this->value = value;
}

template <typename E>
E listNode<E>::getValue() const {
    return value;
}

// 链表迭代器
template <typename E>
class listIter {
public:
    listNode<E>::listNode* next;
    int direction;
};

template <typename T>
class doubleList {
public:
    // 返回链表的长度
    virtual int listLength() const = 0;
    // 链表是否为空
    virtual bool listEmpty() const = 0;
    // 释放所有内存空间
    virtual void listRelease() = 0;
    // 返回当前节点的前一个结点
    virtual listNode<T>* listPrevNode(listNode<T>* node) const = 0;
    // 返回当前节点的后一个节点
    virtual listNode<T>* listNextNode(listNode<T>* node) const = 0;
    // 返回当前节点的值
    virtual T listNodeValue(listNode<T>* node) const = 0;
    // 头插
    virtual void listAddNodeHead(const T &value) = 0;
    // 尾插
    virtual void listAddNodeTail(const T &value) = 0;
    // 任意位置插入
    virtual void listInsertNode(const T &value,int index) = 0;
    // 删除节点
    virtual void listDelNode(const T &value) = 0;
    // 根据值找到节点
    virtual listNode<T>* listSearchKey(const T &value) const = 0;
    // 根据索引找到节点
    virtual listNode<T>* listSearchIndex(int index) const = 0;
    // 虚析构函数 释放内存
    virtual ~doubleList() {};
};

template <typename T>
class linkedList: public doubleList<T> {
private:
    listNode<T>* head;
    listNode<T>* tail;
    int currentLength;
    // 辅助函数 找到某个位置上的节点
    listNode<T>* move(int index);
public:
    linkedList();
    int listLength() const;
    bool listEmpty() const;
    void listRelease();
    listNode<T>* listPrevNode(listNode<T>* node) const;
    listNode<T>* listNextNode(listNode<T>* node) const;
    T listNodeValue(listNode<T>* node) const;
    void listAddNodeHead(const T &value);
    void listAddNodeTail(const T &value);
    void listInsertNode(const T &value,int index);
    void listDelNode(const T &value);
    listNode<T>* listSearchKey(const T &value) const;
    listNode<T>* listSearchIndex(int index) const;
    ~linkedList();
};

template <typename T>
listNode<T>* linkedList<T>::move(int index) {

}

template <typename T>
linkedList<T>::linkedList() {

}

template <typename T>
linkedList<T>::~linkedList() {

}


template <typename T>
int linkedList<T>::listLength() const {

}

template <typename T>
bool linkedList<T>::listEmpty() const {

}

template <typename T>
void linkedList<T>::listRelease() {

}

template <typename T>
listNode<T>* linkedList<T>::listPrevNode(listNode<T> *node) const {

}

template <typename T>
listNode<T>* linkedList<T>::listNextNode(listNode<T> *node) const {

}

template <typename T>
T linkedList<T>::listNodeValue(listNode<T> *node) const {

}

template <typename T>
void linkedList<T>::listAddNodeHead(const T &value) {

}

template <typename T>
void linkedList<T>::listAddNodeTail(const T &value) {

}

template <typename T>
void linkedList<T>::listInsertNode(const T &value, int index) {

}

template <typename T>
void linkedList<T>::listDelNode(const T &value) {

}

template <typename T>
listNode<T> *linkedList<T>::listSearchKey(const T &value) const {

}

template <typename T>
listNode<T> *linkedList<T>::listSearchIndex(int index) const {

}

#endif //TINY_KV_DB_LINKED_LIST_H
