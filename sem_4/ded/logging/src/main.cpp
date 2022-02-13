#include <cstdlib>


#include "observer/observed.h"
#include "micro_logger/MicroLogger.h"


void observed_int_log(const Observed<int> &obj) {
    printf("%d", obj.get_data());
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
    MicroLogger<int> microlog({true});

    Observed<int>::get_default_pool().push_observer([&microlog](const OperatorSignal<int> &signal){
        microlog.log(signal);
    });

    func();

    return 0;
}
