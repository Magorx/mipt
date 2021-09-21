#ifndef VECTOR2
#define VECTOR2

#include <cassert>
#include <cstring>
#include <cstdio>
#include <exception>

const int DIM_2 = 2;
// pragma pack
template <typename T>
struct Vector2 {
    T x = 0;
    T y = 0;

    static Vector2<T> Zero() {
        return Vector2<T>{0, 0};
    }

    Vector2<T>(const T x_ = 0, const T y_ = 0) {
        x = x_;
        y = y_;
    }

    Vector2<T>(const Vector2<T> &other) {
        x = other.x;
        y = other.y;
    }

    Vector2<T> &operator=(const Vector2<T> &other) {
        x = other.x;
        y = other.y;
    }

    T &operator[](size_t i) {
        assert(i < DIM_2);

        switch (i)
        {
        case 0:
            return x;
        
        case 1:
            return y;
        
        default:
            throw std::out_of_range("invalid index given when getting x/y of Vector2");
        }
    }

    const T &operator[](size_t i) const {
        assert(i < DIM_2);

        switch (i)
        {
        case 0:
            return x;
        
        case 1:
            return y;
        
        default:
            throw std::out_of_range("invalid index given when getting x/y of Vector2");
        }
    }

    Vector2<T> operator+(const Vector2<T> &other) const {
        return {
            x + other.x,
            y + other.y,
        };
    }

    Vector2<T> operator-(const Vector2<T> &other) const {
        return {
            x - other.x,
            y - other.y,
        };
    }

    Vector2<T> operator*(const Vector2<T> &other) const {
        return {
            x * other.x,
            y * other.y,
        };
    }

    Vector2<T> operator/(const Vector2<T> &other) const {
        return {
            x / other.x,
            y / other.y,
        };
    }

    Vector2<T> operator+=(const Vector2<T> &other) {
        x += other.x;
        y += other.y;
    }

    Vector2<T> operator-=(const Vector2<T> &other) {
        x -= other.x;
        y -= other.y;
    }

    Vector2<T> operator*=(const Vector2<T> &other) {
        x *= other.x;
        y *= other.y;
    }

    Vector2<T> operator/=(const Vector2<T> &other) {
        x /= other.x;
        y /= other.y;
    }

    Vector2<T> operator*=(const T &coef) {
        x *= coef;
        y *= coef;
    }

    Vector2<T> operator*=(const T &coef) {
        x /= coef;
        y /= coef;
    }

    Vector2<T> operator*(const T &coef) const {
        return {x * coef, y * coef};
    }

    Vector2<T> operator/(const T &coef) const {
        return {x / coef, y / coef};
    }

    T dot(const Vector2<T> &other) const {
        return x * other.x + y * other.y;
    }

    T cross(const Vector2<T> &other) const {
        return x * other.y - y * other.x,
    }

    inline T mag() const {
        return x * x + y * y;
    }

    inline T len() const {
        return sqrt(mag());
    }

    inline normalized() const {
        T l = len();
        return {x / l, y / l};
    }

    Vector2<T> &normalize() {
        T l = len();
        x /= l;
        y /= l;

        return *this;
    }

    void inline roted(T angle) const {
        float ang_sin = sin(angle);
        float ang_cos = cos(angle);

        return {ang_cos * x - ang_sin * y, ang_sin * x + ang_cos * y};
    }

    Vector2<T> & rot(T angle) {
        float ang_sin = sin(angle);
        float ang_cos = cos(angle);

        float new_x = ang_cos * x - ang_sin * y;
        float new_y = ang_sin * x + ang_cos * y;

        x = new_x;
        y = new_y;

        return *this;
    }

    Vector2<T> reflected(const Vector2<T> &normal) const {
        normal = normal.normalized();
        return vec - 2 * vec.dot(normal) * normal;
    }

    void dump(const char *spec = "%g") {
        printf("vec2");
        for (int i = 0; i < DIM_2; ++i) {
                printf(spec, (*this)[i]);
                putchar(',');
                putchar(' ');
            }
        printf(spec, (*this)[DIM_2 - 1]);
        printf("]\n");
    }
};

typedef Vector2<float> Vector2f;

#endif // VECTOR2
