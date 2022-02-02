#pragma once


#include "highlighter.h"
#include "magnetic.h"

#include "utils/interpolator/catmullrom.h"

#include <vector>


const double PX_SPLINE_DOT = 10;
const double PX_SPLINE_MAG_RAD = 10;
const double SPLINE_GRANULARITY = 10;


class SplineDotChangeAcceptor;
class SplineSpawnNewDot;
class SplineUncaptureDot;


class v_Spline : public v_Highlighter {
    friend SplineDotChangeAcceptor;
    friend SplineSpawnNewDot;
    friend SplineUncaptureDot;

    std::vector<v_Magnetic*> dots;
    std::vector<double> output;

    AppearenceTexture *dot_appr;

    RGBA curve_color;
    double mag_radius;

    bool dot_captured = false;

    v_Magnetic *try_spawn_dot(const Vec2d &pos);
    bool try_delete_dot(const Vec2d &pos);

public:
    v_Spline(const ViewBody &body, RGBA curve_color = {255, 0, 0, 255}, RGBA background_color = {0, 0, 0, 0}, double mag_radius = PX_SPLINE_MAG_RAD);
    virtual ~v_Spline();
    
    void recalculate_output();

    virtual void render(Renderer *renderer) override;
};

class SplineSpawnNewDot : public EventAcceptor<v_Spline, Event::MousePress> {
    friend v_Spline;
public:
    SplineSpawnNewDot(v_Spline *spline);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class SplineUncaptureDot : public EventAcceptor<v_Spline, Event::MouseRelease> {
    friend v_Spline;
public:
    SplineUncaptureDot(v_Spline *spline);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class SplineDotChangeAcceptor : public EventAcceptor<v_Spline, Event::FractionChanged> {
    friend v_Spline;
public:
    SplineDotChangeAcceptor(v_Spline *spline);

    EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult *cur_res = nullptr) override;
};

