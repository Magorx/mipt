#pragma once


#include "interpolator.h"


class Interpolator2dCatmullRom : public Interpolator<double, Vec2d> {
    void add_to_data(const Vec2d &point);

    double get_t(double t, double alpha, const Vec2d& p1, const Vec2d& p2);

    Vec2d get_p(const Vec2d &p1, const Vec2d &p2, const Vec2d &p3, const Vec2d &p4, double t, double alpha = 0.5);

public:
    bool swap_coords;

    Interpolator2dCatmullRom(bool to_swap_coords = false, size_t init_len = 8);

    virtual void add_p(const Vec2d &point) override;

    virtual Vec2d operator[](const double &x) override;
};
