#include "func_logger.h"

#include "utils/logger.h"


const int FUNC_CALL_OFFSET_SHIFT = 4;


FuncLogger::FuncLogger(const std::string &name) :
name(name)
{
    if (call_cnt.contains(name)) {
        ++call_cnt[name];
    } else {
        call_cnt[name] = 1;
    }

    signal_dispatcher.dispatch({this->name, call_cnt[name], true});
}

FuncLogger::~FuncLogger() {
    signal_dispatcher.dispatch({this->name, call_cnt[name], false});
    --call_cnt[name];
}

SignalDispatcher<FuncCallSignal> &FuncLogger::get_signal_dispatcher() {
    return signal_dispatcher;
}

std::map<std::string, int> FuncLogger::call_cnt;
SignalDispatcher<FuncCallSignal> FuncLogger::signal_dispatcher;
