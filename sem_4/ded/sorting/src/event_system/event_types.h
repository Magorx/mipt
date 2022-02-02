#pragma once

#include <vector>
#include <cstring>

#include "utils/vec2d.h"
#include "visual/color/rgba.h"
#include "device/keyboard.h"

class Renderer;

namespace Event {

enum MouseButton {
    none,
    left,
    right,
    middle,

    MAX
};

struct ControlKeysData {
    bool alt;
    bool ctrl;
    bool shift;
};

struct MousePress {
    Vec2d position;
    MouseButton button = MouseButton::none;
};

struct MouseRelease {
    Vec2d position;
    MouseButton button = MouseButton::none;
};

struct MouseMove {
    Vec2d from;
    Vec2d to;
};

struct MouseDrag {
    enum Button {
        left,
        right
    };

    Vec2d from;
    Vec2d to;
    Button button;
};

struct Tick {
    double dt;
};

struct RenderCall {
    Renderer *renderer;
};

struct Activator {
    enum State {
        off = 1 << 0,
        on  = 1 << 1,
        toggle = 1 << 2,
        visualy = 1 << 3,
    };


    State mode = State::toggle;
    void *target = nullptr;

    Activator(State mode = State::toggle, void *target = nullptr) : mode(mode), target(target) {}

    inline bool check_target(void *acceptor) const { return !target || target == acceptor; }
};

struct Close {
    int mode = 0;
};

struct ActivityToggle : public Activator {
    using Activator::Activator;
};

struct Clicked {
    
};

struct FractionChanged {
    Vec2d fraction;
};

struct VectorFractionChanged {
    const std::vector<double> &data;
    double max_coef;
};

struct TextChanged {
    const char *text;
    size_t len;

    double val_d;
    int    val_i;

    bool input_complete = false;

    TextChanged(const char *text, bool input_complete = false) :
    text(text),
    len(text ? strlen(text) : 0),
    val_d(NAN),
    val_i(0),
    input_complete(input_complete)
    {
        if (!text) return;

        char *next = nullptr;
        val_d = strtod(text, &next);
        if (*next != '\0') {
            val_d = NAN;
            val_i = 0;
            return;
        }

        val_i = static_cast<int>(val_d);
    }

    TextChanged(const char *text, size_t len, bool input_complete = false, double val_d = NAN, int val_i = 0) :
    text(text),
    len(len),
    val_d(val_d),
    val_i(val_i),
    input_complete(input_complete)
    {}
};

struct Scroll {
    Vec2d delta;
    Vec2d position;
};

struct DataPtr {
    void *receiver;
    void *data;

    inline bool check_receiver(void *obj) const { return !receiver || receiver == obj; }
};

struct ColorChanged {
    RGBA color;
};

}
