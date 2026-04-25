// Vec4.h
# ifndef MyMath_Vec4_H
# define MyMath_Vec4_H

# include <cmath>
# include <cassert>
# include "Vec3.h"

namespace MyMath {
    template <typename T>
    struct Vec4 {
        T x, y, z, w;

        // 构造函数
        Vec4() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
        explicit Vec4(T s) : x(s), y(s), z(s), w(s) {}
        Vec4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
        Vec4(const Vec3<T>& v, T w_ = T(1)) : x(v.x), y(v.y), z(v.z), w(w_) {}

        // 基本运算
        Vec4 operator+(const Vec4& v) const {
            return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
        }
        Vec4 operator-(const Vec4& v) const {
            return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
        }
        Vec4 operator*(T s) const {
            return Vec4(x * s, y * s, z * s, w * s);
        }
        Vec4 operator/(T s) const {
            T invS = T(1) / s; // 除法的倒数
            return *this * invS;
        }

        T dot(const Vec4& v) const {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }

        // 投影到3维空间
        Vec3<T> project() const {
            if (w != 0) {
                return Vec3<T>(x / w, y / w, z / w);
            }
            return Vec3<T>(x, y, z);
        }
    };
    using Vec4f = Vec4<float>;
    using Vec4d = Vec4<double>;
}

# endif