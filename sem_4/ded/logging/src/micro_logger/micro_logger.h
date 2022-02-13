#pragma once


#include "observer/observed.h"


struct MicroLoggerSettings {
    bool to_show_history = false;
};


template<typename T>
class MicroLogger {
    MicroLoggerSettings settings;

public:
    MicroLogger(MicroLoggerSettings settings={}) :
    settings(settings)
    {}

    bool is_importand_operator(Operator op) {
        return op == Operator::asgn_copy || op == Operator::ctor_copy;
    }

    void log(const OperatorSignal<T> &signal) {
        logger.print_log_prefix("mclg", "observed");
        
        if (is_importand_operator(signal.op)) {
            logger.print("op [\n");

            logger.print_log_prefix("mclg", "[--op--]");
            logger.print("    %s\n", to_str(signal.op));

            logger.print_log_prefix("mclg", "[--op--]");
            logger.print("    ");
            logger.print_n_spaces(strlen(to_str(signal.op)));
            logger.print("] {");
        } else {
            logger.print("op [%s] {", to_str(signal.op));
        }
        
        if (!signal.first && !signal.second) {
            logger.print("}\n");
            return;
        } else {
            logger.print("\n");
        }

        if (signal.first) {
            logger.print_log_prefix("mclg", "observed");
            logger.print("    ");
            signal.first->logger_log(settings.to_show_history);
        }

        if (signal.second) {
            logger.print_log_prefix("mclg", "observed");
            logger.print("    ");
            signal.second->logger_log(settings.to_show_history);
        }

        logger.print_log_prefix("mclg", "observed");
        logger.print("}\n");
        logger.n();
    }
};
