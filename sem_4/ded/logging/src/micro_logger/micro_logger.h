#pragma once


#include "observer/observed.h"
#include "micro_logger/func_logger.h"


struct MicroLoggerSettings {
    bool to_show_history = false;

    bool to_log_console = true;
    bool to_log_html = false;
    bool to_log_graphvis = false;

    inline bool console_mode() {
        return to_log_console;
    }

    inline bool html_mode() {
        return !console_mode() && to_log_html;
    }
};


template<typename T>
class MicroLogger {
    Logger &logger;

    MicroLoggerSettings settings;

    void log_important_operator(const OperatorSignal<T> &signal) {
        logger.print("op [\n");

        if (settings.html_mode()) {
            logger.tag("b");
            logger.tag("font")("color", "FF2222");
        }

        logger.print_log_prefix("mclg", "op");

        logger.print("    %s\n", to_str(signal.op));

        if (settings.html_mode()) {
            logger.tag_close(2);
        }

        logger.print_log_prefix("mclg", "op");
        logger.print("    ");
        logger.print_n_spaces(strlen(to_str(signal.op)));
        logger.print("] {");
    }

    void log_normal_operator(const OperatorSignal<T> &signal) {
        logger.print("op [%s] {", to_str(signal.op));
    }

public:
    MicroLogger(Logger &logger=kctf::logger, MicroLoggerSettings settings={}) :
    logger(logger),
    settings(settings)
    {}

    bool is_importand_operator(Operator op) {
        return op == Operator::asgn_copy || op == Operator::ctor_copy;
    }

    void log_opeartor_console(const OperatorSignal<T> &signal) {
        logger.print_log_prefix("mclg", "observed");
        
        if (is_importand_operator(signal.op)) {
            log_important_operator(signal);
        } else {
            log_normal_operator(signal);
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

    void log_operator_html(const OperatorSignal<T> &signal) {
        logger.tag("pre");

        log_opeartor_console(signal);

        logger.tag_close();
    }

    void log_operator(const OperatorSignal<T> &signal) {
        if (settings.to_log_console) {
            log_opeartor_console(signal);
        } else if (settings.to_log_html) {
            logger.tag("pre");

            log_opeartor_console(signal);

            logger.tag_close();
        }
    }

    void log_func(const FuncCallSignal &signal) {
        if (settings.to_log_console) {
            log_func_console(signal);
        } else if (settings.to_log_html) {
            logger.tag("pre");

            log_func_console(signal);

            logger.tag_close();
        }
    }

    void log_func_console(const FuncCallSignal &signal) {
        if (settings.html_mode()) {
            logger.tag("b");
            logger.tag("font")("color", "3333CC");
        }

        if (signal.is_called) {

            logger.print_log_prefix("mclg", "func");
            logger.print("[%d] %s() <{\n", signal.recursion_cnt, signal.name.c_str());
            logger.n();

            logger.shift_offset(+FUNC_CALL_OFFSET_SHIFT);
        } else {
            logger.shift_offset(-FUNC_CALL_OFFSET_SHIFT);

            logger.print_log_prefix("mclg", "func");
            logger.print("}> [%d] %s()\n", signal.recursion_cnt, signal.name.c_str());
        }

        if (settings.html_mode()) {
            logger.tag_close(2);
        }
    }

    Logger &get_logger() {
        return logger;
    }
};
