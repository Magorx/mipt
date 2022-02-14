#include <cstdlib>
#include <cstdio>


#include "observer/observed.h"
#include "micro_logger/MicroLogger.h"


void observed_int_log(const Observed<int> &obj, Logger &logger=kctf::logger) {
    logger.print_aligned(Logger::Align::left, obj.get_max_value_len(), "%d", obj.get_data());
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


void func(int d = 3) {
    FuncLogger flg("func");

    if (!d) return;

    OBSERVED(int, a, 1);
    OBSERVED(int, b, 99);
    OBSERVED(int, caca, 3);
    a = b + caca;

    func(d - 1);

    b = a = caca;
}


int main() {
    logger.set_verb_level(Logger::Level::info);

    Observed<int>::set_one_line_log(observed_int_log);
    Observed<int>::set_value_length(observed_int_length);

    Logger *html_logger = new Logger("log.html");
    html_logger->set_html_mode(true);

    MicroLogger<int> microlog(*html_logger, {true, false, true});

    Observed<int>::get_default_pool().push_observer([&microlog](const OperatorSignal<int> &signal) {
        microlog.log_operator(signal);
    });
    
    FuncLogger::get_signal_dispatcher().push_observer([&microlog](const FuncCallSignal &signal) {
        microlog.log_func(signal);
    });

    func();

    delete html_logger;

    return 0;
}
