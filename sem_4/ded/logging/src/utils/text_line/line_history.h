#pragma once


#include <vector>
#include "utils/state_history/history.h"


class Line;


class LineHisotryState : public HistoryState {
protected:
    Line *line;
    std::vector<char> str;
    int from;
    int to;

public:
    bool prev_space = false;

    LineHisotryState(Line *line, int from, int to);

    virtual void undo() = 0;
    virtual void redo() = 0;
};


class LineHisotryStateInsertion : public LineHisotryState {
public:

    LineHisotryStateInsertion(Line *line, int from, int to);

    void add(char c);

    void undo() override final;
    void redo() override final;
};

class LineHisotryStateDeletion : public LineHisotryState {
    bool right;
public:
    LineHisotryStateDeletion(Line *line, int pos, int len, bool right = true);

    void undo() override final;
    void redo() override final;
};
