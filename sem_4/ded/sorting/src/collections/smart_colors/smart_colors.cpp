#include "smart_colors.h"


SmartColorSin::SmartColorSin(const Color color_,
                             const double freq_,
                             const double time_shift_,
                             const double ampl_) :
color(color_),
ampl(ampl_),
freq(freq_),
time_shift(time_shift_),
cur_time(0)
{}

const Color SmartColorSin::rgb() {
    return color * ampl * fabs(sin(cur_time * freq + time_shift));
}

void SmartColorSin::tick(double, double time) {
    cur_time = time;
}

SmartColorBlip::SmartColorBlip(const Color color_,
                const double time_on_ ,
                const double time_off_,
                const int  init_state ) :
color(color_),
state(init_state),
time_on(time_on_),
time_off(time_off_),
time_cnt(0)
{}

const Color SmartColorBlip::rgb() {
    return color * state;
}

void SmartColorBlip::tick(double dt, double) {
    time_cnt += dt;
    if ((state && time_cnt > time_on) || (!state && time_cnt > time_off)) {
        time_cnt = 0;
        state ^= 1;
    }
}


// SmartColorTimedGradient::SmartColorTimedGradient(const std::vector<SmartColor*> &colors_, 
//                         const double time_span_) :
// colors(colors_),
// time_span(time_span_)
// // cur_time(0)
// {}

// const Color SmartColorTimedGradient::rgb() {
//     double t = fmod(time, time_span) / time_span * colors.size();
//     int idx1 = (int) t % colors.size();
//     int idx2 = (idx1 + 1) % colors.size();
//     double wt = t - std::floor(t);
//     double w1 = 1 - wt;
//     double w2 = wt;
//     return colors[idx1]->rgb(time, x, y) * w1 + colors[idx2]->rgb(time, x, y) * w2;
// }

// SmartColorCheckered::SmartColorCheckered(SmartColor* color_1_,
//                     SmartColor* color_2_, 
//                     const double w_,
//                     const double h_) :
// color_1(color_1_),
// color_2(color_2_),
// w(w_),
// h(h_)
// {
//     if (w == 0) {
//         w = 0.000001;
//     }
//     if (h == 0) {
//         h = 0.000001;
//     }
// }

// const Color SmartColorCheckered::rgb() {
//     int w_cnt = x / w;
//     int h_cnt = y / h;
//     int cnt = w_cnt + h_cnt;
//     if (cnt % 2) {
//         return color_1->rgb(time, x, y);
//     } else {
//         return color_2->rgb(time, x, y);
//     }
// }
