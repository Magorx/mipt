#include "canvas_history.h"
#include "redactor/canvas/canvas.h"


CanvasHistoryState::CanvasHistoryState(Canvas *canvas, int layer_idx, Layer *from, Layer *to) :
canvas(canvas),
delta(nullptr),
from(nullptr),
to(nullptr),
layer_idx(layer_idx)
{
    if (!canvas || !from || !to) {
        return;
    }

    // delta = new Layer(from->renderer, canvas, from->size);

    this->from = new Layer(from->renderer, canvas, from->size);
    this->to   = new Layer(to->renderer, canvas, to->size);
    
    // from->flush_to(delta, true, false, RBlend::none);
    // to->flush_to(delta, true, false, RBlend::me_sub);

    from->flush_to(this->from, true, false, RBlend::none);
    to->flush_to(this->to, true, false, RBlend::none);
}

CanvasHistoryState::~CanvasHistoryState() {
    delete delta;
    delete from;
    delete to;
}

void CanvasHistoryState::undo() {
    if (!canvas || !from) {
        return;
    }

    canvas->set_active_layer(layer_idx);

    from->flush_to(canvas->get_active_layer(), true, false, RBlend::none);
    canvas->upd_prev_active_layer();
}

void CanvasHistoryState::redo() {
    if (!canvas || !to) {
        return;
    }

    canvas->set_active_layer(layer_idx);

    to->flush_to(canvas->get_active_layer(), true, false, RBlend::none);
    canvas->upd_prev_active_layer();
}
