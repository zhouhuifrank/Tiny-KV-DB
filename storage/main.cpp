/*
 * this.FrankZhou
 * 测试使用
 */
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "skip_list.h"

using namespace std;

int main() {
    Node<string,int> node("frankzhou",1,2);
    cout << "key:" << node.getKey() << " : " << "value:" << node.getValue() << endl;
    return 0;
}