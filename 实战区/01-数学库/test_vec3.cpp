# include <iostream>
# include "include/Vec3.h"
using namespace std;
using namespace MyMath;

int main() {
    Vec3f a(1, 2, 3);
    Vec3f b(4, 5, 6);

    // 测试算术运算
    Vec3f c = a + b;
    cout << "a + b = (" << c.x << ", " << c.y << ", " << c.z << ")" << endl;

    // 测试点积
    float dotResult = a.dot(b);
    cout << "a dot b = " << dotResult << endl;

    // 测试叉积
    Vec3f crossResult = a.cross(b);
    cout << "a cross b = (" << crossResult.x << ", " << crossResult.y << ", " << crossResult.z << ")" << endl;

    // 测试单位化
    Vec3f normalized = a.normalize();
    cout << "a normalized = (" << normalized.x << ", " << normalized.y << ", " << normalized.z << ")" << endl;
    cout << "a normalized length = " << normalized.length() << endl;
    
    return 0;
}