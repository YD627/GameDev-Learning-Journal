# include <iostream>
# include "include/Vec3.h"
# include "include/Mat4.h"
using namespace MyMath;
using namespace std;

int main() {
    // 1. 单位矩阵测试
    Mat4f identity = Mat4f::identity();
    cout << "Identity矩阵:" << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << identity(i, j) << " ";
        }
        cout << endl;
    }

    // 2. 平移矩阵测试
    Mat4f translate = Mat4f::translate(Vec3f(1, 2, 3));
    Vec3f points(0, 0, 0);
    Vec3f translated = translate.transformPoint(points);
    cout << "Original Point: (" << points.x << ", " << points.y << ", " << points.z << ")" << endl;
    cout << "Translated Point: (" << translated.x << ", " << translated.y << ", " << translated.z << ")" << endl;

    // 3. 缩放矩阵测试
    Mat4f scale = Mat4f::scale(Vec3f(2, 3, 4));
    Vec3f scaled = scale.transformPoint(Vec3f(1,1,1));
    cout << "Original Point: (" << 1 << ", " << 1 << ", " << 1 << ")" << endl;
    cout << "Scaled Point: (" << scaled.x << ", " << scaled.y << ", " << scaled.z << ")" << endl;

    // 4. 旋转矩阵测试
    Mat4f rotate = Mat4f::rotateZ(3.1415926f / 2.0f);
    Vec3f rotated = rotate.transformPoint(Vec3f(1, 0, 0));
    cout << "Original Point: (" << 1 << ", " << 0 << ", " << 0 << ")" << endl;
    cout << "Rotated Point: (" << rotated.x << ", " << rotated.y << ", " << rotated.z << ")" << endl;

    // 5. 矩阵乘法测试
    Mat4f combined = translate * scale;
    cout << "Combined Matrix:" << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << combined(i, j) << " ";
        }
        cout << endl;
    }

    // 6. 视图矩阵测试（lookAt函数）
    Vec3f eye(0, 0, 5);
    Vec3f target(0, 0, 0);
    Vec3f up(0, 1, 0);
    
    Mat4f view = Mat4f::LookAt(eye, target, up);
    Vec3f worldPoint(0, 0, 0);
    Vec3f viewPoint = view.transformPoint(worldPoint);
    
    std::cout << "\n6. 视图矩阵测试 (LookAt):\n";
    std::cout << "摄像机位置: (" << eye.x << ", " << eye.y << ", " << eye.z << ")\n";
    std::cout << "观察点: (" << target.x << ", " << target.y << ", " << target.z << ")\n";
    std::cout << "世界坐标(0,0,0)在视图空间: (" << viewPoint.x << ", " << viewPoint.y << ", " << viewPoint.z << ")\n";
    std::cout << "期望: (0, 0, -5) 左右\n";



    std::cout << "\n=== 投影矩阵测试 ===\n";
    
    // 正交投影测试
    Mat4f ortho = Mat4f::orthographic(-1, 1, -1, 1, 0.1f, 100.0f);
    std::cout << "1. 正交投影矩阵创建成功\n";
    
    // 透视投影测试
    Mat4f persp = Mat4f::perspective(3.1415926f/4.0f, 16.0f/9.0f, 0.1f, 100.0f);
    std::cout << "2. 透视投影矩阵创建成功\n";
    
    // 测试点在投影空间中的变换
    Vec3f point(0, 0, -5);  // 在摄像机前方5单位
    Vec4f projPoint = persp * Vec4f(point, 1);
    Vec3f projected = projPoint.project();
    
    std::cout << "3. 透视投影测试:\n";
    std::cout << "原始点: (" << point.x << ", " << point.y << ", " << point.z << ")\n";
    std::cout << "投影后: (" << projected.x << ", " << projected.y << ", " << projected.z << ")\n";
}
