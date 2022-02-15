#include <cstdlib>
#include <cstdio>


#include "micro_logger/MicroLogger.h"


std::string observed_int_log(const Observed<int> &obj) {
    return std::to_string(obj.get_data());
}

int observed_int_length(const Observed<int> &obj) {
    int val = obj.get_data();
    if (!val) return 1;
    
    int cnt = 0;
    while (val && ++cnt) {
        val /= 10;
    }

    return cnt;
}


void func(int = 3) {
    FuncLogger flg("func");

    OBSERVED(int, a, 5);
    OBSERVED(int, b, 99);
    OBSERVED(int, c, 99);
    // OBSERVED(int, b, 99);
    a = b + (c++);

    // if (!d) return;

    // OBSERVED(int, a, 1);
    // OBSERVED(int, b, 99);
    // OBSERVED(int, name, 3);
    // a = b + name;
    // a = a + name;

    // func(d - 1);

    // b = a = name;
}


int main() {
    logger.set_verb_level(Logger::Level::info);

    Observed<int>::set_one_line_log(observed_int_log);
    Observed<int>::set_value_length(observed_int_length);

    Logger *html_logger = new Logger("log.html");
    html_logger->set_html_mode(true);

    MicroLogger<int> microlog(*html_logger, {true, false, true});
    MicroLoggerGraph<int> micrograph;

    Observed<int>::get_default_pool().push_observer([&microlog, &micrograph](const OperatorSignal<int> &signal) {
        microlog.log_operator(signal);
        micrograph.log_operator(signal);
    });
    
    FuncLogger::get_signal_dispatcher().push_observer([&microlog, &micrograph](const FuncCallSignal &signal) {
        microlog.log_func(signal);
        micrograph.log_func(signal);
    });

    func();

    microlog.end_log();
    delete html_logger;

    micrograph.graph();

    return 0;
}
