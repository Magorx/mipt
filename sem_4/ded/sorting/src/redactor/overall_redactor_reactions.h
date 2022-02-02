#pragma once


#include "event_system/event_system.h"
#include "collections/views/magnetic.h"

#include "redactor/tools/tool_manager.h"


class ToolManagerHotkeys : public EventAcceptor<ToolManager, Event::KeyDown> {
    int prev_index = 0;

public:
    ToolManagerHotkeys(ToolManager *tool_manager) :
    EventAcceptor(tool_manager)
    {}

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult*) override {
        int prev = acceptor->get_active_tool_idx();

        Hotkey prev_hotkey = acceptor->get_active_tool()->get_hotkey();

        acceptor->set_active_tool(Keyboard::make_hotkey(event.code));

        Hotkey cur_hotkey = acceptor->get_active_tool()->get_hotkey();

        if (cur_hotkey != prev_hotkey) {
            prev_index = prev;
        }

        return EventAccResult::cont;
    }
};


class ToolManagerAltToolOff : public EventAcceptor<ToolManager, Event::KeyUp> {
public:
    int prev_idx = 0;

    ToolManagerAltToolOff(ToolManager *tool_manager) :
    EventAcceptor(tool_manager)
    {}

    EventAccResult operator()(const Event::KeyUp &event, const EventAccResult*) override {
        if (Keyboard::make_hotkey(event.code) != HotkeyBind::alt_active_tool) return EventAccResult::none;
        
        acceptor->set_active_tool(prev_idx);

        return EventAccResult::none;
    }
};


class ToolManagerAltToolOn : public EventAcceptor<ToolManager, Event::KeyDown> {
    ToolManagerAltToolOff *restorer;
public:
    ToolManagerAltToolOn(ToolManager *tool_manager, ToolManagerAltToolOff *restorer) :
    EventAcceptor(tool_manager),
    restorer(restorer)
    {}

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult*) override {
        if (Keyboard::make_hotkey(event.code) != HotkeyBind::alt_active_tool) return EventAccResult::none;
        
        restorer->prev_idx = acceptor->get_active_tool_idx();
        acceptor->set_active_tool(HotkeyBind::pipette);

        return EventAccResult::none;
    }
};


class ToolManagerScrollShiftToolSize : public EventAcceptor<ToolManager, Event::Scroll> {
    double granularity;

public:
    ToolManagerScrollShiftToolSize(ToolManager *slider, double granularity = 0.5) :
    EventAcceptor(slider),
    granularity(granularity)
    {}

    EventAccResult operator()(const Event::Scroll &event, const EventAccResult*) override {
        acceptor->shift_size(granularity * event.delta.y());
        acceptor->update_active_tool();

        return EventAccResult::done;
    }
};
