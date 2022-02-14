#include <cstdlib>
#include <cstdio>


#include "observer/observed.h"
#include "micro_logger/MicroLogger.h"


void observed_int_log(const Observed<int> &obj, Logger logger=kctf::logger) {
    logger.print("%d", obj.get_data());
}


void func(int d = 3) {
    FuncLogger flg("func");

    if (!d) return;

    OBSERVED(int, a, 1);
    OBSERVED(int, b, 2);
    OBSERVED(int, c, 3);
    a = b + c;

    func(d - 1);
}


int main() {
    logger.set_verb_level(Logger::Level::info);

    Observed<int>::set_one_line_log(observed_int_log);

    FILE *file = fopen("log.log", "w");
    MicroLogger<int> microlog({file}, {true});

    Observed<int>::get_default_pool().push_observer([&microlog](const OperatorSignal<int> &signal) {
        microlog.log_operator(signal);
    });
    
    FuncLogger::get_signal_dispatcher().push_observer([&microlog](const FuncCallSignal &signal) {
        microlog.log_func(signal);
    });

    func();

    fclose(file);

    return 0;
}
