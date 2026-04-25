# include <iostream>
using namespace std;

float linearInterpolation(float x1, float x2, float y1, float y2, float x) {
    float a = (y2 - y1) / (x2 - x1);
    float b = y1 - a * x1;
    return a * x + b;
}

float bilinearInterpolation(
    float x, float y, // 要插值的点
    float x1, float x2, float y1, float y2, // x和y的坐标范围
    float f11, float f12, float f21, float f22 // 四个点的值Q11(x1, y1),Q12(x1, y2), Q21(x2, y1), Q22(x2, y2)
) {
    float R1 = linearInterpolation(x1, x2, f11, f21, x); // 计算Q11到Q21的线性插值(x方向插值)
    float R2 = linearInterpolation(x1, x2, f12, f22, x); // 计算Q12到Q22的线性插值(x方向插值)
    return linearInterpolation(y1, y2, R1, R2, y); // 计算R1到R2的线性插值(y方向插值)
}

int main() {
    float x1 = 0, x2 = 5, y1 = 2, y2 = 12;
    cout << "当x = 3时, 单线性插值结果为: " << linearInterpolation(x1, x2, y1, y2, 3) << endl;
    cout << "当x = 3, y = 4时, 双线性插值结果为: " << bilinearInterpolation(3, 4, x1, x2, y1, y2, 3, 4, 5, 6) << endl;
    return 0;
}