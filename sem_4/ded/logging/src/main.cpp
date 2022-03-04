#include <cstdlib>
#include <cstdio>

#include "micro_logger/MicroLogger.h"
#include "utils/move_forward.h"


std::string observed_string_log(const Observed<std::string> &obj) {
    return obj.get_data();
}

int observed_string_length(const Observed<std::string> &obj) {
    return (int) obj.get_data().length();
}


using Type = Observed<std::string>;


template<typename T>
void to_uppercase(T &&obj) {
    FuncLogger flg("TO_UPERCASE");

    my_remove_reference_t<T> tmp = my_forward<T>(obj);

    for (size_t i = 0; i  < tmp.get_data().length(); ++i) {
        char c = tmp.get_data()[i];
        if (c >= 'a' && c <= 'z') {
            c += 'A' - 'a';
        }

        tmp.get_data()[i] = c;
    }
}


template<typename T>
void recieve_and_pass(T &&obj) {
    FuncLogger flg("Gona pass");

    to_uppercase(obj);
}

template<typename T>
void recieve_and_move(T &&obj) {
    FuncLogger flg("Gona move");

    to_uppercase(my_move(obj));
}

template<typename T>
void recieve_and_forward(T &&obj) {
    FuncLogger flg("Gona forward");

    to_uppercase(my_forward<T>(obj));
}

void func() {
    FuncLogger flg("func");

    OBSERVED(std::string, initial, "Feature Film");
    recieve_and_forward(initial);

    if (initial.get_data().length() == 0) {
        initial.get_data() = "~~~Snatch~~~";
    }
}


int main() {
    logger.set_verb_level(Logger::Level::info);

    Observed<std::string>::set_one_line_log(observed_string_log);
    Observed<std::string>::set_value_length(observed_string_length);

    Logger *html_logger = new Logger("log.html");
    html_logger->set_html_mode(true);

    MicroLogger<std::string> microlog(*html_logger, {true, false, true});
    MicroLoggerGraph<std::string> micrograph;

    Observed<std::string>::get_default_pool().push_observer([&microlog, &micrograph](const OperatorSignal<std::string> &signal) {
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
