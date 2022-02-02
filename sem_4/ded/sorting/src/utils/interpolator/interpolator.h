#pragma once


#include <vector>
#include <cmath>

#include "utils/vec2d.h"


template <typename ARG_T, typename POINT_T>
class Interpolator {
public:
    std::vector<POINT_T> data;

    Interpolator(size_t init_len = 8) : data() { data.reserve(init_len); }

    virtual void add_p(const POINT_T &point) = 0;

    virtual POINT_T operator[](const ARG_T &arg) = 0;
};

