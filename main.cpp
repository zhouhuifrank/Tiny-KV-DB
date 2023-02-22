#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    srand((unsigned)time(nullptr));

    int level = 0;

    int r = rand()%100 + 1;
    while (r < 50) {
        level++;
        r = rand()%100 + 1;
        cout << r << endl;
    }

    cout << "random level:" << level << endl;

    cout << "zhouhui" << "\n";
    cout << "haha";

    return 0;
}
