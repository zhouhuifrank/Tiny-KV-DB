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
template <typename K,typename V>
class Node {
public:
    K key;
    V value;
    Node<K,V>* prev;
    Node<K,V>* next;

    Node(int key,int value,Node<K,V>* prev=nullptr,Node<K,V>* next=nullptr);
    Node(Node<K,V>* prev,Node<K,V>* next);
    ~Node();
};

template <typename K,typename V>
Node<K,V>::Node(int key, int value, Node<K, V> *prev, Node<K, V> *next) {
    this->key = key;
    this->value = value;
    this->prev = prev;
    this->next = next;
}

template <typename K,typename V>
Node<K,V>::Node(Node<K,V>* prev,Node<K,V>* next) {
    this->prev = prev;
    this->next = next;
}

template <typename K,typename V>
Node<K,V>::~Node() {
}

// 双向链表定义
template <typename K,typename V>
class cacheList {
public:
    virtual bool isEmpty() const = 0;
    virtual int length() const = 0;
    virtual void release() = 0;
    virtual void addFromLast(Node<K,V>* node) = 0;
    virtual Node<K,V>* removeFromFirst() = 0;
    virtual void remove(Node<K,V>* node) = 0;
    virtual ~cacheList() {};
};

template <typename K,typename V>
class DoubleLinkList: public cacheList<K,V> {
private:
    Node<K,V>* head;
    Node<K,V>* tail;
    int currentLength;
    Node<K,V>* move(int i) const;
public:
    DoubleLinkList();
    ~DoubleLinkList();
    bool isEmpty() const;
    int length() const;
    void release();
    void addFromLast(Node<K,V>* node);
    Node<K,V>* removeFromFirst();
    void remove(Node<K,V>* node);
};

template <typename K,typename V>
DoubleLinkList<K,V>::DoubleLinkList() {
    head->next = tail;
    tail->prev = head;
    currentLength = 0;
}

template <typename K,typename V>
Node<K,V>* DoubleLinkList<K,V>::move(int i) const {
    Node<K,V>* curr = head->next;

    while (i--) {
        curr = curr->next;
    }

    return curr;
}

template <typename K,typename V>
DoubleLinkList<K,V>::~DoubleLinkList() {
    release();
    delete head;
    delete tail;
}

template <typename K,typename V>
bool DoubleLinkList<K,V>::isEmpty() const {
    return this->currentLength == 0;
}

template <typename K,typename V>
int DoubleLinkList<K,V>::length() const {
    return this->currentLength;
}

template <typename K,typename V>
void DoubleLinkList<K,V>::release() {
    Node<K,V>* curr = head->next;

    head->next = nullptr;
    while (curr != nullptr) {
        Node<K,V>* next = curr->next;
        delete curr;
        curr = next;
    }
    tail->prev = nullptr;
}

template <typename K,typename V>
void DoubleLinkList<K,V>::addFromLast(Node<K,V>* node) {
    node->prev = tail->prev;
    node->next = tail;
    tail->prev->next = node;
}

template <typename K,typename V>
Node<K, V>* DoubleLinkList<K,V>::removeFromFirst() {
    if (head->next == tail) {
        return;
    }
    Node<K,V>* temp = head->next;
    head->next = head->next->next;
    Node<K,V>* node(temp->key,temp->value);
    delete temp;

    return node;
}

template <typename K,typename V>
void DoubleLinkList<K,V>::remove(Node<K, V> *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
}

// 缓存Cache类定义
template <typename K,typename V>
class cache {
public:
    virtual V get(K key) const = 0;
    virtual void put(K key,V value) = 0;
    virtual ~cache() {};
};

template <typename K,typename V>
class LRUCache: cache<K,V> {
private:
    // 双向链表中的节点编号->节点指针
    std::unordered_map<int,Node<K,V>*> map;
    DoubleLinkList<K,V> cache;
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
    V get(K key) const;
    void put(K key,V value);
};

template <typename K,typename V>
LRUCache<K,V>::LRUCache(int capacity) {
    this->capacity = capacity;
}

template <typename K,typename V>
LRUCache<K,V>::~LRUCache() {
    delete map;
    delete cache;
}

template <typename K,typename V>
V LRUCache<K,V>::get(K key) const {
    if (map.find(key) == map.end()) {
        return nullptr;
    }

    makeRecently(key);
    return map.find(key)->second->value;
}

template <typename K,typename V>
void LRUCache<K,V>::put(K key, V value) {
    if (map.find(key) != map.end()) {
        // 如果不存在就更新
        deleteKey(key);
        addRecently(key,value);
        return;
    }

    // 如果空间满了，就删除最近未使用的
    if (cache.length() == capacity) {
        removeLeastRecently();
    }
    // 如果不存在就新建
    addRecently(key,value);
}

template <typename K,typename V>
void LRUCache<K,V>::makeRecently(int key) {
    if (map.find(key) == map.end()) {
        // 不存在key
        std::cout << "key:" << key << "不存在" << std::endl;
        return;
    }

    auto node =  map.find(key);
    // 从原链表移除，然后在添加到链表尾部  链表头表示最久没有使用的节点 链表尾部表示最近使用的节点
    cache.remove(node);
    cache.addFromLast(node);
}

template <typename K,typename V>
void LRUCache<K,V>::addRecently(int key, int value) {
    Node<K,V>* node(key,value);
    map.insert(std::make_pair(key,node));

    cache.addFromLast(node);
}

template <typename K,typename V>
void LRUCache<K,V>::deleteKey(int key) {
    if (map.find(key) == map.end()) {
        std::cout << "key:" << key << "不存在，不能删除" << std::endl;
        return;
    }

    auto entry = map.find(key);
    Node<K,V>* node =  entry->second;
    map.erase(entry);
    cache.remove(node);
}

template <typename K,typename V>
void LRUCache<K,V>::removeLeastRecently() {
    Node<K,V>* node = cache.removeFromFirst();
    K key = node->key;

    Node<K,V> deleteNode = map.find(key);
    map.erase(deleteNode);
}

#endif //TINY_KV_DB_LRUCACHE_H
