/*
 * this.FrankZhou
 * 跳跃表 底层存储引擎 key-value结构
 */
#ifndef TINY_KV_DB_SKIP_LIST_H
#define TINY_KV_DB_SKIP_LIST_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <mutex>
#include <fstream>

#define STORE_FILE "dumpFile"

std::mutex lockMutex;
std::string delimiter = ":";

// 节点数据类型 key-value形式
template <typename K, typename V>
class Node{
private:
    K key;
    V value;
public:
    // 指针数组的指针，存储当前节点不同层级的索引列表
    Node<K,V> **forward;
    // 当前节点的层高 1-32之间的随机数
    int nodeLevel;
public:
    Node() {};
    Node(K key,V value,int level);
    ~Node();
    K getKey() const;
    V getValue() const;
    void setValue(V value);
};

template <typename K,typename V>
Node<K,V>::Node(K key, V value, int level) {
    this->key = key;
    this->value = value;
    this->nodeLevel = level;

    // 指针数组 index 0-level
    this->forward = new Node<K,V>* [level+1];

    // 初始化forward数组
    memset(this->forward,0,sizeof(Node<K,V>*)*(level+1));
}

template <typename  K,typename V>
Node<K,V>::~Node() {
    delete [] forward;
}

template <typename  K,typename V>
K Node<K, V>::getKey() const {
    return key;
}

template <typename  K,typename V>
V Node<K, V>::getValue() const {
    return value;
}

template <typename  K,typename V>
void Node<K, V>::setValue(V value) {
    this->value = value;
}

// 跳表数据类型
template <typename K,typename V>
class SkipList {
private:
    // 跳表最大层数
    int maxLevel;
    // 跳表当前层数
    int currLevel;
    // 跳表节点个数
    int listLength;
    // 跳表头节点
    Node<K,V>* head;
    // 文件读写
    std::ofstream  fileWriter;
    std::ifstream fileReader;
private:
    void getKeyValueFromString(const std::string &key, const std::string &value);
    bool isValidString(const std::string &str);
public:
    SkipList() {};
    SkipList(int maxLevel);
    ~SkipList();
    int getRandomLevel() const;
    int size() const;
    Node<K,V>* createNode(K key,V value,int level);
    void insertNode(K key,V value);
    void searchNode(K key) const;
    void deleteNode(K key);
    void dumpFile();
    void loadFile();
};

template <typename K,typename V>
SkipList<K,V>::SkipList(int maxLevel) {
    this->maxLevel = maxLevel;
    this->currLevel = 0;
    this->listLength = 0;

    K key;
    V value;
    head = new Node<K,V>(key,value,currLevel);
}

template <typename K,typename V>
SkipList<K,V>::~SkipList() {
    if (fileReader.is_open()) {
        fileReader.close();
    }

    if (fileWriter.is_open()) {
        fileWriter.close();
    }

    delete [] head;
}

// 创建索引节点
template <typename K,typename V>
Node<K, V> *SkipList<K, V>::createNode(K key, V value, int level) {

}

template <typename K,typename V>
void SkipList<K, V>::insertNode(K key, V value) {

}

template <typename K,typename V>
void SkipList<K, V>::searchNode(K key) const {

}

template <typename K,typename V>
void SkipList<K, V>::deleteNode(K key) {

}

// 文件操作
template <typename K,typename V>
void SkipList<K,V>::dumpFile() {

}

template <typename K,typename V>
void SkipList<K,V>::loadFile() {

}


template <typename K,typename V>
int SkipList<K, V>::size() const {
    return listLength;
}

template <typename K,typename V>
int SkipList<K, V>::getRandomLevel() const {

}

template <typename K,typename V>
void SkipList<K,V>::getKeyValueFromString(const std::string &key, const std::string &value) {

}

template <typename K,typename V>
bool SkipList<K,V>::isValidString(const std::string &str) {

}

#endif //TINY_KV_DB_SKIP_LIST_H