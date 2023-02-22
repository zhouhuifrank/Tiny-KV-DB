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
public:
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
    void listJoin(doubleList<T>* source,doubleList<T>* target);
    listNode<T>* listSearchKey(const T &value) const;
    listNode<T>* listSearchIndex(int index) const;
    ~linkedList();
};

template <typename T>
listNode<T>* linkedList<T>::move(int index) {
    listNode<T>* curr = head->next;
    while (index--) {
        curr = curr->next;
    }

    return curr;
}

template <typename T>
linkedList<T>::linkedList() {
    head->next = tail;
    head->preve = nullptr;
    tail->prev = head;
    tail->next = nullptr;
}

template <typename T>
linkedList<T>::~linkedList() {
    listRelease();
    delete head;
    delete tail;
}


template <typename T>
int linkedList<T>::listLength() const {
    return currentLength;
}

template <typename T>
bool linkedList<T>::listEmpty() const {
    return currentLength == 0;
}

template <typename T>
void linkedList<T>::listRelease() {
    listNode<T>* tmp;
    listNode<T>* curr = head->next;
    head->next = nullptr;

    while (curr != nullptr) {
        tmp = curr->next;
        delete curr;
        curr = tmp;
    }
    tail->prev = nullptr;
    currentLength = 0;
}

template <typename T>
listNode<T>* linkedList<T>::listPrevNode(listNode<T> *node) const {
    return node->prev;
}

template <typename T>
listNode<T>* linkedList<T>::listNextNode(listNode<T> *node) const {
    return node->next;
}

template <typename T>
T linkedList<T>::listNodeValue(listNode<T> *node) const {
    return node->getValue();
}

template <typename T>
void linkedList<T>::listAddNodeHead(const T &value) {
    head->next = new listNode<T>(value,head,head->next);
}

template <typename T>
void linkedList<T>::listAddNodeTail(const T &value) {
    tail->prev = new listNode<T>(value,tail->prev,tail);
}

template <typename T>
void linkedList<T>::listInsertNode(const T &value, int index) {
    // index = 0头插   index = currentLength+1 尾插
    if (index < 0 && index > currentLength+1) {
        std::cout << "insert index is invalid" << std::endl;
    }

    if (index == 0) {
        listAddNodeHead(value);
    }
    if (index == currentLength+1) {
        listAddNodeTail(value);
    }

    listNode<T>* prev = move(index-1);
    listNode<T>* insertNode = new listNode<T>(value,prev,prev->next);
    insertNode->next = prev->next;
    prev->next = insertNode;
}

template <typename T>
void linkedList<T>::listDelNode(const T &value) {
    listNode<T>* curr = head->next;

    while (curr != nullptr) {
        if (curr->getValue() != value) {
            curr = curr->next;
        } else {
            break;
        }
    }

    if (curr == nullptr) {
        std::cout << "node value:" << value << std::endl;
    } else {
        listNode<T>* prev = curr->prev;
        listNode<T>* next = curr->next;
        prev->next = next;
        next->prev = prev;
        curr->prev = nullptr;
        curr->next = nullptr;
        delete curr;
    }
}

template <typename T>
void linkedList<T>::listJoin(doubleList<T> *source, doubleList<T> *target) {
    if (source->listLength() == 0) return;
}

template <typename T>
listNode<T>* linkedList<T>::listSearchKey(const T &value) const {
    listNode<T>* curr = head->next;

    while (curr != nullptr) {
        if (curr->getValue() != value) {
            curr = curr->next;
        } else {
            break;
        }
    }

    return curr;
}

template <typename T>
listNode<T> *linkedList<T>::listSearchIndex(int index) const {
    return move(index);
}

#endif //TINY_KV_DB_LINKED_LIST_H
