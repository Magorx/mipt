#include "matrix4.h"

template <typename T>
Matrix4<T>::Matrix4(const T (&_data)[4][4])
{
    memcpy(data, _data, 4*4);
}