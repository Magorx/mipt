#pragma once


#include <deque>

#include "state.h"


class History {
    std::deque<HistoryState*> prev_states;
    std::deque<HistoryState*> next_states;

    size_t max_states_cnt;

    HistoryState *cur_state;

    bool block_add = false;

public:
    History(size_t max_states_cnt);
    ~History();

    void undo();
    void redo();

    void add(HistoryState *state);
    
    void pop_prev();
    void pop_next();

    bool set_add_block(bool block_adding_) { bool prev = block_add; block_add = block_adding_; return prev; }
    bool get_add_block() { return block_add; }

    void update_cur();
    HistoryState *cur() { return cur_state; }
};


class ScopeHistoryAdditionBlocker {
    History &history;
    bool prev_to_block_adding;
    bool restored = false;

public:
    ScopeHistoryAdditionBlocker(History &history) :
    history(history),
    prev_to_block_adding(history.get_add_block())
    {
        history.set_add_block(true);   
    }

    ~ScopeHistoryAdditionBlocker() {
        if (!restored) history.set_add_block(prev_to_block_adding);
    }

    void restore() {
        restored = true;
        history.set_add_block(prev_to_block_adding);
    }
};
