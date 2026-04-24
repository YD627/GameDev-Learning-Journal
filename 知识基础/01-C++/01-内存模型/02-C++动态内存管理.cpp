#include <cstdlib>
#include <iostream>
using namespace std;

class Test{
    private:
        static int nextid;
        int id;
    public:
        Test(){
            id = nextid++;
            cout << "Test(" << id << ")" << endl;
        }
        ~Test(){
            cout << "~Test(" << id << ")" << endl;
        }
};
int Test::nextid = 0;
int main(){
    // 使用new来申请一个int类型的内存，并将空间中数据初始化为10，并将首地址保存到指针ptr1中
    int *p1 = new int(10);
    cout << *p1 << endl;
    delete p1; // 释放p1指向的内存空间

    // 使用new给对象分配空间，自动调用构造函数，使用delete释放内存空间，自动调用析构函数
    Test *p2 = new Test();
    delete p2;

    // 使用new[num]申请num个连续的同类型空间并使用{}中的数据进行初始化
    int *p3 = new int[5]{1,2,3,4,5};
    cout<< p3[3] << endl;
    delete[] p3;

    // 使用new[num]给num个对象初始化空间，使用delete释放指针指向的多个对象
    Test *p4 = new Test[3];
    delete[] p4;

    // 使用malloc/free申请和释放内存
    int *p5 = (int *)malloc(sizeof(int)); // 分配内存，但是不初始化内存
    *p5 = 100; // 初始化内存
    cout << *p5 << endl;
    free(p5);

    // 使用malloc分配数组内存
    int *p6 = (int *)malloc(sizeof(int) * 5);
    for(int i = 0; i < 5; i++){
        p6[i] = i;
    }
    for(int i = 0; i < 5; i++){
        cout << p6[i] << endl;
    }
    free(p6);
}