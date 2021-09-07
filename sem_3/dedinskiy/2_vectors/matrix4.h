#ifndef MATRIX4
#define MATRIX4

#include <cassert>
#include <cstring>
#include <cstdio>

const unsigned int DIM_4 = 4;

template <typename T>
struct Matrix4 {
    T data[4][4];

    static Matrix4<T> Zero() {
        Matrix4<T> ret({{0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}});
        
        return ret;
    }

    static Matrix4<T> Unit() {
        Matrix4<T> ret({{1, 0, 0, 0},
                        {0, 1, 0, 0},
                        {0, 0, 1, 0},
                        {0, 0, 0, 1}});
        
        return ret;
    }
    
    Matrix4<T> ScaleMatrix(float sx, float sy) {
        return Matrix4<T>({{sx, 0,  0},
                           {0,  sy, 0},
                           {0,  0,  1}});
    }

    Matrix4<T> RotationMatrix(float angle) {
        float sine   = sin(angle);
        float cosine = cos(angle);
        return Matrix4<T>({{cosine, -sine,   0},
                           {sine,    cosine, 0},
                           {0,       0,      1}});
    }

    Matrix4<T> TranslationMatrix(float tx, float ty) {
        return Matrix4<T>({{1,  0,  0},
                           {0,  1,  0},
                           {tx, ty, 1}});
    }

    Matrix4<T>() {
        memset(data, 0, DIM_4 * DIM_4 * sizeof(T));
    }

    Matrix4<T>(const T (&_data)[DIM_4][DIM_4]) {
        memcpy(data, _data, DIM_4 * DIM_4 * sizeof(T));
    }

    T *operator[](size_t i) {
        assert(i < DIM_4);

        return data[i];
    }

    const T *operator[](size_t i) const {
        assert(i < DIM_4);

        return data[i];
    }

    Matrix4<T> operator+(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM_4; ++i) {
            for (int j = 0; j < DIM_4; ++j) {
                ret[i][j] = data[i][j] + other[i][j];
            }
        }
        
        return ret;
    }

    Matrix4<T> operator-(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM_4; ++i) {
            for (int j = 0; j < DIM_4; ++j) {
                ret[i][j] = data[i][j] - other[i][j];
            }
        }
        
        return ret;
    }

    Matrix4<T> operator*(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM_4; ++i) {
            for (int j = 0; j < DIM_4; ++j) {
                ret[i][j] = data[i][j] * other[i][j];
            }
        }
        
        return ret;
    }

    Matrix4<T> operator/(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM_4; ++i) {
            for (int j = 0; j < DIM_4; ++j) {
                ret[i][j] = data[i][j] / other[i][j];
            }
        }
        
        return ret;
    }

    Matrix4<T> operator*(const T &coef) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM_4; ++i) {
            for (int j = 0; j < DIM_4; ++j) {
                ret[i][j] = data[i][j] * coef;
            }
        }
        
        return ret;
    }

    Matrix4<T> scale(float sx, float sy) {
        return mul(ScaleMatrix(sx, sy));
    }

    Matrix4<T> rotate(float angle) {
        return mul(RotationMatrix(angle));
    }

    Matrix4<T> mul(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM_4; ++i) {
            for (int j = 0; j < DIM_4; ++j) {
                T sum = 0;
                for (int k = 0; k < DIM_4; ++k) {
                    sum = sum + data[i][k] * other[k][j];
                }
                
                ret[i][j] = sum;
            }
        }

        return ret;
    }

    void dump(const char *spec) {
        for (int i = 0; i < DIM_4; ++i) {
            for (int j = 0; j < DIM_4; ++j) {
                printf(spec, data[i][j]);
            }
            printf("\n");
        }
    }
};

typedef Matrix4<float> Matrix4f;

#endif // MATRIX4
