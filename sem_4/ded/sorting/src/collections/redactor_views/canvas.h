#pragma once


#include "utils.h"
#include "redactor/canvas/canvas.h"
#include "visual/renderer/renderer.h"

class CanvasPressAcceptor;
class CanvasReleaseAcceptor;
class CanvasMoveAcceptor;
class CanvasHotkeyAcceptor;


class v_Canvas : public v_Highlighter {
    Canvas *canvas;

    friend CanvasPressAcceptor;
    friend CanvasReleaseAcceptor;
    friend CanvasMoveAcceptor;
    friend CanvasHotkeyAcceptor;

public:
    v_Canvas(const ViewBody &body, Renderer *renderer, ToolManager *tool_manager);
    virtual ~v_Canvas();

    virtual void render(Renderer *renderer) override;

    inline Canvas *get_canvas() { return canvas; }
};


class CanvasPressAcceptor : public EventAcceptor<v_Canvas, Event::MousePress> {
    friend v_Canvas;
public:
    CanvasPressAcceptor(v_Canvas *button);
    virtual ~CanvasPressAcceptor();

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class CanvasReleaseAcceptor : public EventAcceptor<v_Canvas, Event::MouseRelease> {
    friend v_Canvas;
public:
    CanvasReleaseAcceptor(v_Canvas *button);
    virtual ~CanvasReleaseAcceptor();

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class CanvasMoveAcceptor : public EventAcceptor<v_Canvas, Event::MouseMove> {
    friend v_Canvas;
public:
    CanvasMoveAcceptor(v_Canvas *button);
    virtual ~CanvasMoveAcceptor();

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};

class CanvasHotkeyAcceptor : public EventAcceptor<v_Canvas, Event::KeyDown> {
    friend v_Canvas;
public:
    CanvasHotkeyAcceptor(v_Canvas *button);
    virtual ~CanvasHotkeyAcceptor();

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult *cur_res = nullptr) override;
};
