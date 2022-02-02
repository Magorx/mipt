
#include <vector>
#include <functional>
#include <cassert>

#include "event_reaction.h"

#include <cstring>

template <typename ES_T, typename EVENT_T>
class EventDispatcher {
    ES_T *es;
    const char  *id;

    std::vector<EventReaction<EVENT_T>*> observers_before;
    std::vector<EventReaction<EVENT_T>*> observers_after;

    typedef std::function<EVENT_T(const EVENT_T &)> EventAffector;
    EventAffector event_affector;

public:

    EventDispatcher(ES_T *es, const char *id = "noname_event") : es(es), id(id), event_affector(nullptr) {
        assert(es && "can't create an EventDispatcher without parental EventSystem");
    }

    virtual ~EventDispatcher() {
        for (size_t i = 0; i < observers_before.size(); ++i) {
            delete observers_before[i];
        }
        for (auto observer : observers_after) {
            delete observer;
        }
    }

    EventDispatcher &operator=(const EventDispatcher &other) = delete;

    void add(EventReaction<EVENT_T> *observer, bool before_sub_pass = true) {
        if (before_sub_pass) observers_before.push_back(observer);
        else observers_after.push_back(observer);
    }

    virtual EventAccResult emit(const EVENT_T &event, bool sub_es_reverse = false, bool is_signal = false) {
        if (event_affector) {
            EVENT_T affected_event = event_affector(event);
            return clear_sub_result(dispatch(affected_event, sub_es_reverse, is_signal));
        } else {
            return clear_sub_result(dispatch(event, sub_es_reverse, is_signal));
        }
    }

    EventAccResult dispatch_to_observers(const EVENT_T &event, bool before_sub_pass = true, const EventAccResult *cur_res = nullptr) {
        EventAccResult sub_res = EventAccResult::none;
        auto &observers = before_sub_pass ? observers_before : observers_after;
    
        for (auto observer : observers) {
            EventAccResult res = (*observer)(event, cur_res);
            process_acc_result(res, sub_res);
            if ((res & EventAccResult::done) || (res & EventAccResult::stop)) { return sub_res; }
        }

        return sub_res;
    }

    EventAccResult dispatch_to_sub_es(const EVENT_T &event, bool sub_es_reverse = false);

    void process_acc_result(EventAccResult &res, EventAccResult &sub_res);
    EventAccResult clear_sub_result(EventAccResult sub_res);

    EventAccResult dispatch(const EVENT_T &event, bool sub_es_reverse, bool is_signal = false) {
        EventAccResult sub_res = EventAccResult::none;

        EventAccResult res = dispatch_to_observers(event);
        process_acc_result(res, sub_res);
        if ((res & EventAccResult::done) || (res & EventAccResult::stop)) {
            return sub_res;
        }
        sub_res = clear_sub_result(sub_res);

        if (!is_signal) {
            EventAccResult sub_sys_res = dispatch_to_sub_es(event, sub_es_reverse);
            process_acc_result(sub_sys_res, sub_res);
            if (sub_res & EventAccResult::done) {
                return sub_res;
            }
            sub_res = clear_sub_result(sub_res);

            res = dispatch_to_observers(event, false, &sub_sys_res);
        } else {
            res = dispatch_to_observers(event, false, nullptr);
        }

        process_acc_result(res, sub_res);
        if ((res & EventAccResult::done) || (res & EventAccResult::stop)) {
            return sub_res;
        }

        return clear_sub_result(sub_res);
    }

    const char *get_id() { return id; }

    void set_event_affector(EventAffector affector) { event_affector = affector; }

    inline void pop_observer(bool before_sub_pass = true) {
        auto &observers = before_sub_pass ? observers_before : observers_after;

        if (!observers.size()) return;

        delete observers[observers.size() - 1];
        observers.pop_back();
    }
};
