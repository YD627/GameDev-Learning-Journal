// Mat4.h
// 4x4矩阵类，用于图形学变换
// 采用列主序(Column-Major Order)存储，与OpenGL的矩阵存储顺序一致

# ifndef MyMath_Mat4_H
# define MyMath_Mat4_H

# include "Vec3.h"
# include "Vec4.h"
# include <array>
# include <cmath>
# include <cassert>
# include <cstring>

namespace MyMath {
    template <typename T>
    class Mat4 {
        private:
            T m[4][4];
        
        public:
            // ========== 构造函数 ==========
            // 默认构造函数
            Mat4() {identity();}

            // 用16个值初始化
            explicit Mat4(T m00, T m10, T m20, T m30,
                          T m01, T m11, T m21, T m31,
                          T m02, T m12, T m22, T m32,
                          T m03, T m13, T m23, T m33) {
                m[0][0] = m00; m[1][0] = m10; m[2][0] = m20; m[3][0] = m30;
                m[0][1] = m01; m[1][1] = m11; m[2][1] = m21; m[3][1] = m31;
                m[0][2] = m02; m[1][2] = m12; m[2][2] = m22; m[3][2] = m32;
                m[0][3] = m03; m[1][3] = m13; m[2][3] = m23; m[3][3] = m33;
            }

            // 用数组初始化
            explicit Mat4(const T* m) {
                memcpy(this->m, m, sizeof(T) * 16);
                assert(this->m != nullptr);
            }

            // ========== 特殊矩阵构造 ==========
            // 单位矩阵
            static Mat4 identity() {
                return Mat4(
                          1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0);
            }

            // 置为单位矩阵
            static void identity(Mat4& mat) {
                mat = Mat4::identity();
            }

            // 零矩阵
            static Mat4 zero() {
                return Mat4(
                          0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0);
            }

            // ========== 访问器 ==========
            // 访问矩阵元素(列，行)
            T& operator()(int col, int row) {
                return m[col][row];
            }

            const T& operator()(int col, int row) const {
                return m[col][row];
            }

            // 获取列向量
            Vec4<T> getColum(int col) const {
                return Vec4<T>(m[col][0], m[col][1], m[col][2], m[col][3]);
            }

            // 获取行向量
            Vec4<T> getRow(int row) const {
                return Vec4<T>(m[0][row], m[1][row], m[2][row], m[3][row]);
            }

            // 设置列向量
            void setColum(int col, const Vec4<T>& v) {
                m[col][0] = v.x; m[col][1] = v.y; m[col][2] = v.z; m[col][3] = v.w;
            }

            // 设置行向量
            void setRow(int row, const Vec4<T>& v) {
                m[0][row] = v.x; m[1][row] = v.y; m[2][row] = v.z; m[3][row] = v.w;
            }

            // ========== 基本运算 ==========
            Mat4 operator+(const Mat4& mat) const {
                return Mat4(
                          m[0][0] + mat[0][0], m[1][0] + mat[1][0], m[2][0] + mat[2][0], m[3][0] + mat[3][0],
                          m[0][1] + mat[0][1], m[1][1] + mat[1][1], m[2][1] + mat[2][1], m[3][1] + mat[3][1],
                          m[0][2] + mat[0][2], m[1][2] + mat[1][2], m[2][2] + mat[2][2], m[3][2] + mat[3][2],
                          m[0][3] + mat[0][3], m[1][3] + mat[1][3], m[2][3] + mat[2][3], m[3][3] + mat[3][3]);
            }

            // 减法运算符
            Mat4 operator-(const Mat4& mat) const {
                return Mat4(
                          m[0][0] - mat.m[0][0], m[1][0] - mat.m[1][0], m[2][0] - mat.m[2][0], m[3][0] - mat.m[3][0],
                          m[0][1] - mat.m[0][1], m[1][1] - mat.m[1][1], m[2][1] - mat.m[2][1], m[3][1] - mat.m[3][1],
                          m[0][2] - mat.m[0][2], m[1][2] - mat.m[1][2], m[2][2] - mat.m[2][2], m[3][2] - mat.m[3][2],
                          m[0][3] - mat.m[0][3], m[1][3] - mat.m[1][3], m[2][3] - mat.m[2][3], m[3][3] - mat.m[3][3]);
            }

            // 矩阵乘法运算
            Mat4 operator*(const Mat4& mat) const {
                Mat4 result = Mat4::zero();
                for(int i = 0; i < 4; i++) {
                    for(int j = 0; j < 4; j++) {
                        for(int k = 0; k < 4; k++) {
                            result.m[i][j] += m[k][j] * mat.m[i][k];
                        }
                    }
                }
                return result;
            }

            // 矩阵乘标量
            Mat4 operator*(T s) const {
                Mat4 result;
                for(int i = 0; i < 4; i++) {
                    for(int j = 0; j < 4; j++) {
                        result.m[i][j] = m[i][j] * s;
                    }
                }
                return result;
            }

            // 矩阵除标量
            Mat4 operator/(T s) const {
                T invS = T(1) / s; // 除法的倒数
                return *this * invS;
            }

            // 矩阵乘以向量
            Vec4<T> operator*(const Vec4<T>& v) const {
                return Vec4<T>(
                     m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
                     m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
                     m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
                     m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w
                );
            }

            // 矩阵乘以Vec3(自动补充w=1),用于点的位置变换
            Vec3<T> transformPoint(const Vec3<T>& v) const {
                Vec4<T> result = *this * Vec4<T>(v, 1);
                return Vec3<T>(result.x/result.w, result.y/result.w, result.z/result.w);
            }

            // 矩阵乘以Vec3(自动补充w=0),用于向量的方向变换
            Vec3<T> transformVector(const Vec3<T>& v) const {
                Vec4<T> result = *this * Vec4<T>(v, 0);
                return Vec3<T>(result.x, result.y, result.z);
            }

            // ========== 赋值运算 ==========
            Mat4& operator+=(const Mat4& mat) {
                return *this = *this + mat;
            }
            Mat4& operator-=(const Mat4& mat) {
                return *this = *this - mat;
            }
            Mat4& operator*=(T s) {
                return *this = *this * s;
            }
            Mat4& operator*=(const Mat4& mat) {
                return *this = *this * mat;
            }
            Mat4& operator/=(T s) {
                return *this = *this / s;
            }

            // ========== 比较运算 ==========
            bool operator==(const Mat4& mat) const {
                for(int i = 0; i < 4; i++) {
                    for(int j = 0; j < 4; j++) {
                        if(m[i][j] != mat[i][j]) {
                            return false;
                        }
                    }
                }
                return true;
            }
            bool operator!=(const Mat4& mat) const {
                return !(*this == mat);
            }

            // ========== 矩阵操作 ==========
            // 转置
            Mat4 transpose() const {
                Mat4 result;
                for(int i = 0; i < 4; i++) {
                    for(int j = 0; j < 4; j++) {
                        result.m[i][j] = m[j][i];
                    }
                }
                return result;
            }
            // 行列式和逆矩阵未来实现

            // ========= 变换矩阵生成函数 ==========

            // 平移矩阵
            static Mat4 translate(T x, T y, T z) {
                return Mat4(
                    T(1), T(0), T(0), x,
                    T(0), T(1), T(0), y,
                    T(0), T(0), T(1), z,
                    T(0), T(0), T(0), T(1)
                );
            }
            static Mat4 translate(const Vec3<T>& v) {
                return translate(v.x, v.y, v.z);
            }

            // 缩放矩阵
            static Mat4 scale(T x, T y, T z) {
                return Mat4(
                    x,    T(0), T(0), T(0),
                    T(0), y,    T(0), T(0),
                    T(0), T(0), z,    T(0),
                    T(0), T(0), T(0), T(1)
                );
            }
            static Mat4 scale(const Vec3<T>& v) {
                return scale(v.x, v.y, v.z);
            }
            static Mat4 scale(T s) {
                return scale(s, s, s);
            }

            // 绕X轴旋转
            static Mat4 rotateX(T angle) {
                T c = std::cos(angle);
                T s = std::sin(angle);

                return Mat4(
                    1, 0, 0, 0,
                    0, c, -s, 0,
                    0, s, c, 0,
                    0, 0, 0, 1
                );
            }
            // 绕Y轴旋转
            static Mat4 rotateY(T angle) {
                T c = std::cos(angle);
                T s = std::sin(angle);

                return Mat4(
                    c, 0, s, 0,
                    0, 1, 0, 0,
                    -s, 0, c, 0,
                    0, 0, 0, 1
                );
            }
            // 绕Z轴旋转
            static Mat4 rotateZ(T angle) {
                T c = std::cos(angle);
                T s = std::sin(angle);

                return Mat4(
                    c, -s, 0, 0,
                    s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                );
            }

            // 绕任意轴旋转(使用罗德里格斯旋转公式)
            static Mat4 rotate(T angle, const Vec3<T>& axis) {
                Vec3<T> a = axis.normalized();
                T c = std::cos(angle);
                T s = std::sin(angle);  
                T t = T(1) - c;
        
                T x = a.x, y = a.y, z = a.z;
        
                return Mat4(
                    t*x*x + c,   t*x*y - s*z, t*x*z + s*y, 0,
                    t*x*y + s*z, t*y*y + c,   t*y*z - s*x, 0,
                    t*x*z - s*y, t*y*z + s*x, t*z*z + c,   0,
                    0, 0, 0, 1
                );
            }

            // 正交投影矩阵
            static Mat4 orthographic(T left, T right, T bottom, T top, T near, T far) {
                T rl = right - left;
                T bt = top - bottom;
                T fn = far - near;

                return Mat4(
                    2/rl, 0,    0,    -(left+right)/rl,
                    0,    2/bt, 0,    -(bottom+top)/bt,
                    0,    0,    2/fn, -(near+far)/fn,
                    0,    0,    0,          1
                );
            }
             // 透视投影矩阵(不懂原理，后面要学习)
            static Mat4 perspective(T fovY, T aspectRatio, T near, T far) {
                T f = T(1) / std::tan(fovY / T(2));
                T rangeInv = T(1) / (near - far);
        
                return Mat4(
                    f/aspectRatio, 0, 0, 0,
                    0, f, 0, 0,
                    0, 0, (far + near)*rangeInv, 2*far*near*rangeInv,
                    0, 0, -1, 0
                );
            }
            // 视图矩阵(LookAt)
            static Mat4 LookAt(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up) {
                // 计算前向向量
                Vec3<T> f = (target - eye).normalize();
                Vec3<T> r = f.cross(up).normalize();
                Vec3<T> u = r.cross(f).normalize();
                return Mat4(
                    r.x,   r.y,  r.z, -r.dot(eye),
                    u.x,   u.y,  u.z, -u.dot(eye),
                    -f.x, -f.y, -f.z,  f.dot(eye),
                    0, 0, 0, 1
                );
            }
    };
    // 类型别名
    using Mat4f = Mat4<float>;
    using Mat4d = Mat4<double>;
}


# endif