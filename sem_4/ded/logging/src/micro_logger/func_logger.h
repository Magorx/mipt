#pragma once


#include <string>
#include <map>

#include "event_system/signal.h"


extern const int FUNC_CALL_OFFSET_SHIFT;


struct FuncCallSignal {
    const std::string &name;
    int recursion_cnt;
    bool is_called;
};


class FuncLogger {
    std::string name;

    static std::map<std::string, int> call_cnt;

    static SignalDispatcher<FuncCallSignal> signal_dispatcher;

public:
    FuncLogger(const std::string &name);
    ~FuncLogger();

    static SignalDispatcher<FuncCallSignal> &get_signal_dispatcher();
};
