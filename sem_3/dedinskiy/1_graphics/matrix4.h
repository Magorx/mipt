#ifndef MATRIX4
#define MATRIX4

#include <cassert>
#include <cstring>
#include <cstdio>

const unsigned char DIM = 4;

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

    Matrix4<T>() {
        memset(data, 0, DIM * DIM * sizeof(T));
    }

    Matrix4<T>(const T (&_data)[DIM][DIM]) {
        memcpy(data, _data, DIM * DIM * sizeof(T));
    }

    T *operator[](size_t i) {
        assert(i < DIM);

        return data[i];
    }

    const T *operator[](size_t i) const {
        assert(i < DIM);

        return data[i];
    }

    Matrix4<T> operator+(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                ret[i][j] = data[i][j] + other[i][j];
            }
        }
        
        return ret;
    }

    Matrix4<T> operator-(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                ret[i][j] = data[i][j] - other[i][j];
            }
        }
        
        return ret;
    }

    Matrix4<T> operator*(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                ret[i][j] = data[i][j] * other[i][j];
            }
        }
        
        return ret;
    }

    Matrix4<T> operator/(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                ret[i][j] = data[i][j] / other[i][j];
            }
        }
        
        return ret;
    }

    Matrix4<T> operator*(const T &coef) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                ret[i][j] = data[i][j] * coef;
            }
        }
        
        return ret;
    }

    Matrix4<T> mul(const Matrix4<T> &other) {
        Matrix4<T> ret;
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                T sum = 0;
                for (int k = 0; k < DIM; ++k) {
                    sum = sum + data[i][k] * other[k][j];
                }
                
                ret[i][j] = sum;
            }
        }

        return ret;
    }

    void dump(const char *spec) {
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                printf(spec, data[i][j]);
            }
            printf("\n");
        }
    }
};

typedef Matrix4<float> Matrix4f;

#endif // MATRIX4
