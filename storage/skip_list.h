/*
 * this.FrankZhou
 * 跳跃表模板类 底层存储引擎 key-value结构
 */
#ifndef TINY_KV_DB_SKIP_LIST_H
#define TINY_KV_DB_SKIP_LIST_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <mutex>
#include <fstream>
#include <time.h>

#define STORE_FILE "/store/dumpFile"
#define SKIP_LIST_P 0.5
#define MAX_MEMORY 1024*1024*100;

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
    std::ofstream fileWriter;
    std::ifstream fileReader;
private:
    void getKeyValueFromString(const std::string& str,std::string* key, std::string* value);
    bool isValidString(const std::string &str);
    int calculateMemory();
public:
    SkipList() {};
    SkipList(int maxLevel);
    ~SkipList();
    // 插入节点的时候获取一个随机的层高
    int getRandomLevel() const;
    // 返回跳表的大小
    int size() const;
    // 创建一个层高为level的节点
    Node<K,V>* createNode(K key,V value,int level);
    // 向跳表中插入一个节点
    void insertNode(K key,V value);
    // 按key查询
    bool searchNode(K key) const;
    // 删除key对应的值
    void deleteNode(K key);
    // 打印跳表
    void printAllSkipList();
    // 数据落盘
    void storeFile();
    // 文件加载
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
    return new Node<K,V>*(key,value,level);
}

// 插入、查询和删除是跳表的核心方法
template <typename K,typename V>
void SkipList<K, V>::insertNode(K key, V value) {
    lockMutex.lock();
    Node<K,V> *current = this->head;

    // update数组用于存放每一层需要操作的节点
    Node<K,V>* update[maxLevel+1];
    memset(update,0,sizeof(Node<K,V>*)*(maxLevel+1));

    // 从最高层级开始遍历
    for (int i = currLevel;i>=0;i--) {
        while (current->forward[i] != nullptr && current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    // 节点重复
    if (current != nullptr && current->getKey() == key) {
        std::cout << "key:" << key << "exists" << std::endl;
        lockMutex.unlock();
        return;
    }

    // 节点不重复
    if (current == nullptr || current->getKey() != key) {
        // 产生随机层高
        int randomLevel = getRandomLevel();

        if (randomLevel > currLevel) {
            for (int i=currLevel+1;i<=randomLevel;i++) {
                update[i] = head;
            }
            current = randomLevel;
        }

        Node<K,V>* insertNode = createNode(key,value,randomLevel);

        for (int i=0;i<=randomLevel;i++) {
            insertNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = insertNode;
        }
        std::cout << "insert key success," << "key:" << key << "," << "value:" << value << std::endl;
        listLength++;
    }
    lockMutex.unlock();
    return;
}

template <typename K,typename V>
bool SkipList<K, V>::searchNode(K key) const {
    std::cout << "searchNode-----------------" << std::endl;
    Node<K,V>* current = head;

    for (int i=currLevel;i>=0;i--) {
        while (current->forward[i] && current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != nullptr && current->getKey() == key) {
        std::cout << "Found key:" << key << ", value:" << current->getValue() << std::endl;
        return true;
    }

    std::cout << "Not Found Key:" << key << std::endl;
    return false;
}

template <typename K,typename V>
void SkipList<K, V>::deleteNode(K key) {
    lockMutex.lock();
    Node<K,V>* current = this->head;
    Node<K,V>* update[maxLevel+1];
    memset(update,0,sizeof(Node<K,V>*)*(maxLevel+1));

    for (int i=currLevel;i>=0;i--) {
        while (current->forward[i] != nullptr && current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }

        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->getKey() == key) {
        for (int i=0;i<=currLevel;i++) {
            if (update[i]->forward[i] != current) {
                break;
            }

            update[i]->forward[i] = current->forward[i];
        }

        while (currLevel > 0 && head->forward[currLevel] == 0) {
            currLevel--;
        }

        std::cout << "Success deleted key " << key << std::endl;
        listLength--;
    }
    lockMutex.unlock();
    return;
}

template <typename K,typename V>
void SkipList<K,V>::printAllSkipList() {
    std::cout << "******SkipList******" << std::endl;
    // 遍历跳表的每层
    for (int i=0;i<=currLevel;i++) {
        // 拿到每一层的头节点
        Node<K,V>* node = this->head->forward[i];
        std::cout << "Level " << i << ": ";
        // 遍历每一层
        while (node != nullptr) {
            std::cout << node->getKey() << ":" << node->getValue() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}

// 文件操作
template <typename K,typename V>
void SkipList<K, V>::storeFile() {
    std::cout << "store file---------------------" << std::endl;
    fileWriter.open(STORE_FILE);
    Node<K,V>* node = this->head->forward[0];

    // 将第一层的所有节点写入文件
    while (node != nullptr) {
        fileWriter << node->getKey() << ":" << node->getValue() << "\n";
        std::cout << "key:" << node->getKey() << " " << "value:" << node->getValue() << std::endl;
        node = node->forward[0];
    }

    // 关闭文件流
    fileWriter.flush();
    fileWriter.close();
}

template <typename K,typename V>
void SkipList<K,V>::loadFile() {
    std::cout << "load file------------------------" << std::endl;
    fileReader.open(STORE_FILE);

    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (std::getline(fileReader,line)) {
        // 从字符串中得到键值对
        getKeyValueFromString(line,key,value);
        // 字符串非法
        if (key->empty() || value->empty()) {
            break;
        }
        // 插入跳表中
        std::cout << "key:" << key << " " << "value:" << value << std::endl;
        insertNode(key,value);
    }

    fileReader.close();
}


template <typename K,typename V>
int SkipList<K, V>::size() const {
    return listLength;
}

/*
 * SKIP_LIST_P设定为0.5 50%的节点第一层 25%的节点第二层 12.5%的节点第三层
 * 生成随机的层高
 */
template <typename K,typename V>
int SkipList<K, V>::getRandomLevel() const {
    srand((unsigned)time(nullptr));
    int level = 1;

    int r = rand()%100 + 1;
    while (r < SKIP_LIST_P && level < maxLevel) {
        level++;
    }
    return level;
}

/*
 * 从文件的行中读取key和value的值
 */
template <typename K,typename V>
void SkipList<K,V>::getKeyValueFromString(const std::string &str,std::string* key, std::string* value) {
    if (!isValidString(str)) {
        std::cout << "store file is not correct" << std::endl;
        return;
    }

    *key = str.substr(0,str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1,str.length());
}

// 验证文件中的数据是否正确
template <typename K,typename V>
bool SkipList<K,V>::isValidString(const std::string &str) {
    if (str.empty()) {
        return false;
    }

    // :分隔符必须在中间
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

// 计算当前跳表中的节点存储的内存大小
template <typename  K,typename V>
int SkipList<K,V>::calculateMemory() {
    int byteNumber = sizeof(Node<K,V>*)*listLength;
    return byteNumber;
}

#endif //TINY_KV_DB_SKIP_LIST_H