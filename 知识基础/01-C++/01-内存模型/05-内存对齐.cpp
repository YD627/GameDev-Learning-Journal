#include <iostream>
using namespace std;

struct emptyStruct{};
struct Struct1{
    int a;
    double b;
    char c;
};
struct Struct2{
    char a;
    int b;
    double c;
};
struct alignas(32) Struct3{
    int a;
    double b;
};
class Class1 {
    char a;
    int b;
    char c;
};
class Class2 {
    char a;
    char b;
    int c;
};
class BigData {
    char array[33];
};
class BigData2 {
    BigData bd;
    double d;
};
union Union1 {
    int a;
    double b;
    char c;
};


int main() {
    cout << "bool类型的内存大小: " << sizeof(bool) << endl;
    cout << "char类型的内存大小: " << sizeof(char) << endl;
    cout << "short类型的内存大小: " << sizeof(short) << endl;
    cout << "int类型的内存大小: " << sizeof(int) << endl;
    cout << "float类型的内存大小: " << sizeof(float) << endl;
    cout << "double类型的内存大小: " << sizeof(double) << endl;
    cout << "指针类型的内存大小: " << sizeof(void*) << endl;
    cout << "字符串类型的内存大小: " << sizeof(string) << endl;

    cout << "空结构体类型的内存大小: " << sizeof(emptyStruct) << endl;
    cout << "Struct1类型的内存大小: " << sizeof(Struct1) << endl;
    cout << "Struct2类型的内存大小: " << sizeof(Struct2) << endl;
    cout << "Struct3类型的内存大小: " << sizeof(Struct3) << endl;

    alignas(16) int a;
    cout << "a类型的内存大小: " << sizeof(a) << endl;
    cout << "a类型的内存对齐: " << alignof(a) << endl;

    cout << "Class1类型的内存大小: " << sizeof(Class1) << endl;
    cout << "Class2类型的内存大小: " << sizeof(Class2) << endl;
    cout << "BigData类型的内存大小: " << sizeof(BigData) << endl;
    cout << "BigData2类型的内存大小: " << sizeof(BigData2) << endl;

    cout << "Union1类型的内存大小: " << sizeof(Union1) << endl;
    return 0;
}