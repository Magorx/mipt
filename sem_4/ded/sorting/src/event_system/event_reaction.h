#pragma once


enum EventAccResult {
    none  = 0,
    stop  = 1 << 1,
    cont  = 1 << 2,
    done  = 1 << 3,
    focus = 1 << 4,
    prevent_siblings_dispatch = 1 << 5,
    to_delete = 1 << 6,
};


template <typename EVENT_T>
class EventReaction {
public:
    virtual ~EventReaction() = default;
    virtual EventAccResult operator()(const EVENT_T &event, const EventAccResult *cur_res = nullptr) = 0;
};


template <typename ACCEPTOR_T, typename EVENT_T>
class EventAcceptor : public EventReaction<EVENT_T> {
protected:
    ACCEPTOR_T *acceptor;
public:
    virtual ~EventAcceptor() = default;
    EventAcceptor(ACCEPTOR_T *acceptor) : acceptor(acceptor) {}
};
