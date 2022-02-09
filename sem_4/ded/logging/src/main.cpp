#include <cstdlib>


#include "observer/observed.h"


int main() {
    logger.set_verb_level(Logger::Level::info);

    Observed<int>::get_default_pool().push_observer([](const OperatorSignal<int> &signal){
        int first_id = signal.first ? signal.first->get_id() : -1;
        int second_id = signal.second ? signal.second->get_id() : -1;

        logger.info("obd-int", "op [%+9s] | [%7d] | [%7d]", to_str(signal.op), first_id, second_id);
    });

    Observed<int> a = 1;
    Observed<int> b = 2;

    printf("a + b = %d\n", (a + b).get_data());

    return 0;
}

