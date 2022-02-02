#pragma once


#include "engine/tickable.h"


class AbstractAnimation : public Tickable {
protected:
    double start_time;
    double end_time;
    double cur_time;
    
    double overall_duration;
    double time_coef;
    
    bool running;
    bool looped;

public:
    AbstractAnimation(double overall_duration, bool looped = true, double time_coef = 1) :
    start_time(0),
    end_time(0),
    cur_time(0),
    overall_duration(overall_duration),
    time_coef(time_coef),
    running(false),
    looped(looped)
    {}

    virtual ~AbstractAnimation() {};

    virtual void tick(const double = 0, const double absolute_time = 0) override { cur_time = absolute_time; update(); };

    virtual void start()  { running = true; start_time = cur_time; end_time = start_time + overall_duration; }
    virtual void stop() { running = false; }
    virtual void update() {
        if (cur_time > end_time) {
            if (looped) {
                start();
            } else {
                stop();
            }
        }
    }

    inline double elapsed() const { return cur_time - start_time; }

    inline bool is_running() const { return running; }
    inline void set_looped(bool looped_) { looped = looped_; }
};
