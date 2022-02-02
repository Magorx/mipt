template <typename ES_T, typename EVENT_T>
void EventDispatcher<ES_T, EVENT_T>::process_acc_result(EventAccResult &res, EventAccResult &sub_res) {
    if (res & EventAccResult::focus) {
        es->focus();
        sub_res = (EventAccResult) (sub_res | EventAccResult::focus);
    }
    if (res & EventAccResult::cont) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::cont);
    }
    if (res & EventAccResult::done) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::done);
    }
    if (res & EventAccResult::stop) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::stop);
    }
    if (res & EventAccResult::prevent_siblings_dispatch) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::prevent_siblings_dispatch);
    }
    if (res & EventAccResult::to_delete) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::to_delete);
    }
}

template <typename ES_T, typename EVENT_T>
EventAccResult EventDispatcher<ES_T, EVENT_T>::clear_sub_result(EventAccResult sub_res) {
    sub_res = (EventAccResult) (sub_res & ~EventAccResult::stop);

    return sub_res;
}

template <typename ES_T, typename EVENT_T>
EventAccResult EventDispatcher<ES_T, EVENT_T>::dispatch_to_sub_es(const EVENT_T &event, bool sub_es_reverse) {
    EventAccResult sub_res = EventAccResult::none;

    int idx_start = 0;
    int idx_step = 1;
    int idx_stop = es->get_sub_es().size();
    if (sub_es_reverse) {
        idx_start = (int) es->get_sub_es().size() - 1;
        idx_step = -1;
        idx_stop = -1;
    }

    std::vector<ES_T*> deleted_sub_es;

    auto sub_systems = es->get_sub_es();
    for (int i = idx_start; i != idx_stop; i += idx_step) {
        auto sub_es = sub_systems[i];
        if (sub_es->to_delete) {
            deleted_sub_es.push_back(sub_es);
            continue;
        }

        EventAccResult res = sub_es-> template get_dispatcher<ES_T, EVENT_T>().emit(event, sub_es_reverse);
        process_acc_result(res, sub_res);
        sub_res = clear_sub_result(sub_res);
        if ((res & EventAccResult::done) || (res & EventAccResult::prevent_siblings_dispatch)) return sub_res;
    }

    for (auto sub_es : deleted_sub_es) {
        delete sub_es;
    }

    return sub_res;
}
