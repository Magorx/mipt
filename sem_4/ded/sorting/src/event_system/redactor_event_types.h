#pragma once


#include "visual/color/rgba.h"

#include "redactor/canvas/canvas.h"
#include "redactor/tools/tool.h"


namespace RedactorEvent {

struct ToolColorChanged {
    RGBA color;
    RGBA prev_color;
};

struct NewCanvasOpened {
    Canvas *canvas;
};

struct ActiveToolChanged {
    Tool *tool;
    Tool *prev_tool;
};

}
