#pragma once


#include "utils.h"


class PressPosPrint : public EventReaction<Event::MousePress> {
public:
    virtual EventAccResult operator()(const Event::MousePress &event, const EventAccResult * = nullptr) {
        printf("mouse pressed at %g %g\n", event.position.x(), event.position.y());

        return EventAccResult::none;
    }
};
