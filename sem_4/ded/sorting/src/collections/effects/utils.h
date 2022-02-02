#pragma once


#include "utils/effect.h"
#include "collections/views/window.h"
// #include "engine/moga_engine.h"

class MogaEngine;

class WindowSetter {
public:
    virtual v_Window *create_settings_window(MogaEngine *engine) = 0;
};
