#include "redactor/engine.h"

#include "app_initialization.h"


int main(int argc, char*[]) {
    srand(time(nullptr));
    logger.set_verb_level(Logger::Level::info);

    Observed<int>::get_default_pool().push_observer(log_op_signals);

    Observed<int> a = 7;
    Observed<int> b = 20;

    for (int i = 1; i < 10; ++i) {
        Observed<int> c = a + b / i;
        a = b - c;
    }

    Observed<int>::get_default_pool().pop_observer();

    RWindow *window = nullptr;
    
    if (argc > 1) window = MogaEngine::create_window("MOGADACTOR", true);
    else          window = MogaEngine::create_window("MOGADACTOR", {SCR_W, SCR_H});

    RedactorEngine moga(window, "MOGADACTOR");
    App.init(&moga);

    initialize_photoshop(moga);

    moga.everlasting_loop();

	return 0;
}
