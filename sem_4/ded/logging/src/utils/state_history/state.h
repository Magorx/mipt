#pragma once

class History;

class HistoryState {
protected:
    History *history;
public:
    virtual ~HistoryState() {}
    virtual void undo() = 0;
    virtual void redo() = 0;

    virtual bool empty() { return false; }

    void set_history(History *history_) { history = history_; }
};
