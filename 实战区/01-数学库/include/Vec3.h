// Vec3.h
// 说明：3维向量(Vector3D)类头文件
// 用途：表示空间中的点、方向、颜色(RGB)等

# ifndef MYMATH_VEC3_H // 头文件保护宏，防止重复包含
# define MYMATH_VEC3_H

# include <cmath> // 包含数学函数库
# include <cassert> // 包含断言库

namespace MyMath { // 定义命名空间MyMath，用于存放数学相关的类和函数

    // 使用模板以支持float和double两种精度
    template <typename T>
    struct Vec3 {
        // 数据类型:
        T x, y, z;

        // 1. 构造函数
        // 默认构造函数
        Vec3() : x(T(0)), y(T(0)), z(T(0)) {}

        // 值构造函数：用三个相同的标量初始化
        explicit Vec3(T s) : x(s), y(s), z(s) {}

        // 主构造函数
        Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

        // 2. 下标运算符：提供类似数组的访问方式，便于循环处理
        // const版本用于常量对象
        const T& operator[](int i) const {
            assert(i >= 0 &&  i < 3); // 边界检测，发布时可以注释掉
            return (&x)[i]; // 利用内存的连续分布
        }
        // 非const版本
        T& operator[](int i) {
            assert(i >= 0 &&  i < 3); // 边界检测，发布时可以注释掉
            return (&x)[i]; // 利用内存的连续分布
        }

        // 3. 算术运算符重载(成员函数形式)
        // 向量加法
        Vec3 operator+(const Vec3& v) const {
            return Vec3(x + v.x, y + v.y, z + v.z);
        }
        // 向量减法
        Vec3 operator-(const Vec3& v) const {
            return Vec3(x - v.x, y - v.y, z - v.z);
        }
        // 分量乘法
        Vec3 operator*(const Vec3& v) const {
            return Vec3(x * v.x, y * v.y, z * v.z);
        }
        // 分量除法
        Vec3 operator/(const Vec3& v) const {
            return Vec3(x / v.x, y / v.y, z / v.z);
        }

        // 标量乘法
        Vec3 operator*(T s) const {
            return Vec3(x * s, y * s, z * s);
        }
        // 标量除法
        Vec3 operator/(T s) const {
            T invS = T(1) / s; // 除法的倒数
            return Vec3(x * invS, y * invS, z * invS);
        }

        // 4. 算术运算符重载
        Vec3 operator+=(const Vec3& v) {
            x += v.x; y += v.y; z += v.z;
            return *this;
        }
        Vec3 operator-=(const Vec3& v) {
            x -= v.x; y -= v.y; z -= v.z;
            return *this;
        }
        Vec3 operator*=(const Vec3& v) {
            x *= v.x; y *= v.y; z *= v.z;
            return *this;
        }
        Vec3 operator/=(const Vec3& v) {
            T invS = T(1) / v.x; // 除法的倒数
            x *= invS; y *= invS; z *= invS;
            return *this;
        }
        Vec3 operator*=(T s) {
            x *= s; y *= s; z *= s;
            return *this;
        }
        Vec3 operator/=(T s) {
            T invS = T(1) / s; // 除法的倒数
            x *= invS; y *= invS; z *= invS;
            return *this;
        }

        // 5. 比较运算符
        bool operator==(const Vec3& v) const {
            return x == v.x && y == v.y && z == v.z;
        }
        bool operator!=(const Vec3& v) const {
            return !(*this == v);
        }

        // 6. 核心数学运算
        // 点积(Dot Product, 内积, 内乘, 内积乘)
        T dot(const Vec3& v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        // 叉积(Cross Product, 外积, 外乘, 外积乘)
        Vec3 cross(const Vec3& v) const {
            return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }

        // 向量长度的平法
        T lengthSq() const {
            return x * x + y * y + z * z;
        }
        // 向量长度
        T length() const {
            return std::sqrt(lengthSq());
        }

        // 单位化(归一化): 返回方向相同但长度为1的向量
        Vec3 normalize() const {
            T len = length();
            // 处理零向量的情况
            if (len > T(0)) {
                T invLen = T(1) / len;
                return Vec3(x * invLen, y * invLen, z * invLen);
            }
            return *this; // 零向量的单位化结果为零向量
        }

        // 7. 实用的静态函数
        // 线性插值：Lerp(Linear Interpolation)
        // 用于在两个向量之间进行平滑过渡，根据t的值在0到1之间变化
        static Vec3 lerp(const Vec3& a, const Vec3& b, T t) {
            return Vec3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
        }
        // 距离计算
        static T distance(const Vec3& a, const Vec3& b) {
            return (a - b).length();
        }
        static T distanceSq(const Vec3& a, const Vec3& b) {
            return (a - b).lengthSq();
        }
    };


    // 8. 非成员函数

    // 标量 * 向量的左乘法
    template <typename T>
    Vec3<T> operator*(T s, const Vec3<T>& v) {
        return v * s; // 复用成员函数
    }   


    // 9用函数别名用函数别名(方便使用)
    using Vec3f = Vec3<float>; // 单精度浮点数向量
    using Vec3d = Vec3<double>; // 双精度浮点数向量
    using Pointf = Vec3<float>; // 使用别名来区分点和向量，底层实现一样
    using ColorRGBf = Vec3<float>; // 颜色向量，单精度浮点数

}

# endif