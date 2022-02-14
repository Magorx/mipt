#pragma once


#include "observer/observed.h"
#include "micro_logger/func_logger.h"


struct MicroLoggerSettings {
    bool to_show_history = false;
};


template<typename T>
class MicroLogger {
    Logger logger;

    MicroLoggerSettings settings;

public:
    MicroLogger(Logger logger=kctf::logger, MicroLoggerSettings settings={}) :
    logger(logger),
    settings(settings)
    {}

    bool is_importand_operator(Operator op) {
        return op == Operator::asgn_copy || op == Operator::ctor_copy;
    }

    void log_operator(const OperatorSignal<T> &signal) {
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
            signal.first->logger_log(logger, settings.to_show_history);
        }

        if (signal.second) {
            logger.print_log_prefix("mclg", "observed");
            logger.print("    ");
            signal.second->logger_log(logger, settings.to_show_history);
        }

        logger.print_log_prefix("mclg", "observed");
        logger.print("}\n");
        logger.n();
    }

    void log_func(const FuncCallSignal &signal) {
        if (signal.is_called) {
            logger.print_log_prefix("mclg", "  func  ");
            logger.print("[%d] %s() <{\n", signal.recursion_cnt, signal.name.c_str());
            logger.n();

            logger.shift_offset(+FUNC_CALL_OFFSET_SHIFT);
        } else {
            logger.shift_offset(-FUNC_CALL_OFFSET_SHIFT);

            logger.print_log_prefix("mclg", "  func  ");
            logger.print("}> [%d] %s()\n", signal.recursion_cnt, signal.name.c_str());
        }
    }
};
