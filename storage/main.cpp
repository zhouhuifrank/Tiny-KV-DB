/*
 * this.FrankZhou
 * 测试使用
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include "skip_list.h"

#define DUMP_FILE "storeFile"

using namespace std;

int main() {
    std::ifstream fileReader;
    std::ofstream fileWriter;

    // 写文件
    fileWriter.open(DUMP_FILE,ios::app);
    Node<int,std::string> node1(1,"zhouhui",2);
    Node<int,std::string> node2(3,"xintong",3);
    Node<int,std::string> node3(6,"sb",3);
    Node<int,std::string> node4(2,"hazi",4);

    fileWriter << node1.getKey() << ":" << node1.getValue() << std::endl;
    fileWriter << node2.getKey() << ":" << node2.getValue() << std::endl;
    fileWriter << node3.getKey() << ":" << node3.getValue() << std::endl;
    fileWriter << node4.getKey() << ":" << node4.getValue() << std::endl;

    fileWriter.flush();
    fileWriter.close();

    // 读文件
    fileReader.open(DUMP_FILE);
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (std::getline(fileReader,line)) {
        if (line.empty()) {
            break;
        }

        if (line.find(":") == std::string::npos) {
            std::cout << "format is error" << std::endl;
            break;
        }

        *key = line.substr(0,line.find(":"));
        *value = line.substr(line.find(":")+1,line.size());
        cout << *key << ":" << *value << std::endl;
    }
    fileReader.close();

    // sizeof 以byte为单位
    int memory_size =  sizeof(Node<int,string>)*100;
    cout << "current memory:" << memory_size << endl;
    cout << "max memory:" << MAX_MEMORY;

    return 0;
}