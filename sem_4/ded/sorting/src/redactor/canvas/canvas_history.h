#pragma once


#include "utils/state_history/history.h"


class Canvas;
struct Layer;


class CanvasHistoryState : public HistoryState {
    Canvas *canvas;
    Layer *delta;
    Layer *from;
    Layer *to;
    int layer_idx;

public:
    CanvasHistoryState(Canvas *canvas, int layer_idx, Layer *from, Layer *to);
    ~CanvasHistoryState();

    void undo() override final;
    void redo() override final;
};
