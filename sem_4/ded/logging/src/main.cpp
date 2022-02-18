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


void func(int d = 2) {
    FuncLogger flg("func");
    if (!d) return;

    // OBSERVED(int, a, 3);
    // OBSERVED(int, b, 3);
    // OBSERVED(int, c, 3);
    // OBSERVED(int, d, 3);

    // c = (a + b) * d;

    OBSERVED(int, a, 3);
    OBSERVED(int, b, 3);
    OBSERVED(int, c, 3);

    // func(d - 1);

    b = c * ((a++) + a);
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

    RGBA a("ff33c9");
    LOOG("%s\n", to_string(a).c_str());

    return 0;
}
