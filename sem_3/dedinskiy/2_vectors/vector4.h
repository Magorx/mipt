#ifndef VECTOR4
#define VECTOR4

#include <cassert>
#include <cstring>
#include <cstdio>
#include <exception>

#include "matrix4.h"

template <typename T>
struct Vector4 {
    T x = 0;
    T y = 0;
    T z = 0;
    T w = 0;

    static Vector4<T> Zero() {
        return Vector4<T>{0, 0, 0, 0};
    }

    Vector4<T>(const T x_ = 0, const T y_ = 0, const T z_ = 0, const T w_ = 1) {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }

    Vector4<T>(const Vector4<T> &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    Vector4<T> &operator=(const Vector4<T> &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    T &operator[](size_t i) {
        assert(i < DIM_4);

        switch (i)
        {
        case 0:
            return x;
        
        case 1:
            return y;
        
        case 2:
            return z;
        
        case 3:
            return w;
        
        default:
            throw std::out_of_range("invalid index given when getting x/y/z/w of Vector4");
        }
    }

    const T &operator[](size_t i) const {
        assert(i < DIM_4);

        switch (i)
        {
        case 0:
            return x;
        
        case 1:
            return y;
        
        case 2:
            return z;
        
        case 3:
            return w;
        
        default:
            throw std::out_of_range("invalid index given when getting x/y/z/w of Vector4");
        }
    }

    Vector4<T> operator+(const Vector4<T> &other) {
        return {
            x + other.x,
            y + other.y,
            z + other.z,
            w
        };
    }

    Vector4<T> operator-(const Vector4<T> &other) {
        return {
            x - other.x,
            y - other.y,
            z - other.z,
            w
        };
    }

    Vector4<T> operator*(const Vector4<T> &other) {
        return {
            x * other.x,
            y * other.y,
            z * other.z,
            w
        };
    }

    Vector4<T> operator/(const Vector4<T> &other) {
        return {
            x / other.x,
            y / other.y,
            z / other.z,
            w
        };
    }

    Vector4<T> operator*(const T &coef) {
        return {x * coef, y * coef, z * coef, w};
    }

    Vector4<T> operator/(const T &coef) {
        return {x / coef, y / coef, z / coef, w};
    }

    T dot(const Vector4<T> &other) {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector4<T> cross(const Vector4<T> &other) {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x,
            w
        };
    }

    void dump(const char *spec = "%g") {
        printf("vec4[");
        for (int i = 0; i < DIM_4; ++i) {
                printf(spec, (*this)[i]);
                putchar(',');
                putchar(' ');
            }
        printf(spec, (*this)[DIM_4 - 1]);
        printf("]\n");
    }

    // Matrix operators

    Vector4<T> operator*(const Matrix4<T> &matrix) {
        Vector4<T> res;
        for (int i = 0; i < DIM_4; ++i) {
            res[i] = 0;
            for (int j = 0; j < DIM_4; ++j) {
                res[i] += (*this)[j] * matrix[j][i];
            }
        }

        return res;
    }
};

typedef Vector4<float> Vector4f;
typedef Vector4<char>  ColorRGBA;

#endif // VECTOR4
