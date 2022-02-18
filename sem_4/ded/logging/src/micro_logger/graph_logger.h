#pragma once


#include "settings.h"


#include "observer/observed.h"
#include "micro_logger/func_logger.h"

#include "utils/logger.h"

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>


struct MicroLoggerGraphSettings {

};


template <typename T>
class MicroLoggerGraph {
    std::map<const Observed<T>*, int> obs_to_node;
    std::map<const Observed<T>*, int> init_obs_to_node;

    std::ofstream log_file;
    std::string file_name;

    std::string var_node_format;
    std::string unary_op_node_format;
    std::string binary_op_node_format;
    std::string func_node_format;

    std::string buf;

    int node_cnt = 1;

    bool graphed = false;

public:
    MicroLoggerGraph(const std::string log_file_name="lograph.tmp") :
    log_file(log_file_name),
    file_name(log_file_name)
    {
        logger.set_verb_level(Logger::Level::info);

        std::ifstream tmp_file;
        std::stringstream buffer;

        tmp_file.open("formats/var_node.gv_format");
        buffer << tmp_file.rdbuf();
        var_node_format = buffer.str();
        tmp_file.close();
        buffer.str(std::string());

        tmp_file.open("formats/unary_op.gv_format");
        buffer << tmp_file.rdbuf();
        unary_op_node_format = buffer.str();
        tmp_file.close();
        buffer.str(std::string());

        tmp_file.open("formats/binary_op.gv_format");
        buffer << tmp_file.rdbuf();
        binary_op_node_format = buffer.str();
        tmp_file.close();
        buffer.str(std::string());

        tmp_file.open("formats/func.gv_format");
        buffer << tmp_file.rdbuf();
        func_node_format = buffer.str();
        tmp_file.close();
        buffer.str(std::string());

        buf.resize(1024);

        log_head();
    }

    ~MicroLoggerGraph() {

    }

    inline void clear_buf() {
        buf.assign(buf.size(), 0);
    }

    int get_node(const Observed<T> *observed) {
        if (obs_to_node.contains(observed)) {
            return obs_to_node[observed];
        } else {
            return obs_to_node[observed] = 0;
        }
    }

    int inc_node_idx(const Observed<T> *observed) {
        if (obs_to_node.contains(observed)) {
            return obs_to_node[observed] = ++node_cnt;
        } else {
            return obs_to_node[observed] = 0;
        }
    }

    bool is_creating_op(Operator op) {
        return op == Operator::ctor
            || op == Operator::ctor_copy
            || op == Operator::ctor_move;
    }

    bool is_node_replacing_op(Operator op) {
        return 0
            || op == Operator::asgn_copy
            || op == Operator::asgn_move
            || op == Operator::dtor
            || op == Operator::asgn
            || op == Operator::asgn_add
            || op == Operator::asgn_sub
            || op == Operator::asgn_mul
            || op == Operator::asgn_div
            || op == Operator::asgn_mod
            || op == Operator::asgn_b_and
            || op == Operator::asgn_b_or
            || op == Operator::asgn_b_xor
            || op == Operator::asgn_b_sl
            || op == Operator::asgn_b_sr
            || op == Operator::pref_add
            || op == Operator::pref_sub
            || op == Operator::post_add
            || op == Operator::post_sub;
    }

    void draw_arrow_move(int node_from, int node_to) {
        clear_buf();
        snprintf(buf.data(), buf.size(), "node%d->node%d [style=dashed, constraint=true];\n",
                 node_from,
                 node_to);
        log_file << buf.c_str();
    }

    void draw_arrow_flow(int node_from, int node_to) {
        clear_buf();
        snprintf(buf.data(), buf.size(), "node%d->node%d [constraint=true, color=\"#%s\"];\n",
                 node_from,
                 node_to,
                 to_string(CLR_FLOW).c_str());
        log_file << buf.c_str();
    }
    
    void draw_arrow_actor(int node_from, int node_to, Operator op, std::string label="", bool kostil=true) {
        clear_buf();

        if (is_important_bad(op)) {
            label = "<b><font color=\"#" + to_string(CLR_IMPORTANT_BAD) + "\">COPY</font></b>";
        } else if (is_important_good(op)) {
            if (kostil) {
                label = "<font color=\"#" + to_string(CLR_IMPORTANT_GOOD) + "\">MOVE</font>";
            } else {
                label = "<font color=\"#" + to_string(CLR_IMPORTANT_GOOD) + "\">TO</font>";
            }
        }

        snprintf(buf.data(), buf.size(), "node%d->node%d [constraint=true, label=<%s> color=\"#%s\"];\n",
                 node_from,
                 node_to,
                 label.c_str(),
                 get_op_color(op).c_str());
        log_file << buf.c_str();
    }

    void log_var_node(const Observed<T> *obj) {
        clear_buf();

        std::string color;
        if (obj->is_tmp()) {
            color = '#' + to_string(CLR_VAR_BAD);
        } else {
            color = '#' + to_string(CLR_VAR_NORMAL);
        }

        snprintf(buf.data(), buf.size(), var_node_format.c_str(),
                 ++node_cnt,
                 color.c_str(),
                 obj->get_name().c_str(),
                 color.c_str(),
                 obj->get_id(),
                 color.c_str(),
                 obj,
                 obj->to_str().c_str());

        log_file << buf.c_str();
    }
    
    void log_binary_op(const Observed<T> *first, const Observed<T> *second, Operator op) {
        clear_buf();

        std::string color_first;
        if (first->is_tmp()) {
            color_first = '#' + to_string(CLR_VAR_BAD);
        } else {
            color_first = '#' + to_string(CLR_VAR_NORMAL);
        }

        std::string color_second;
        if (second->is_tmp()) {
            color_second = '#' + to_string(CLR_VAR_BAD);
        } else {
            color_second = '#' + to_string(CLR_VAR_NORMAL);
        }

        snprintf(buf.data(), buf.size(), binary_op_node_format.c_str(),
                 ++node_cnt,
                 color_first.c_str(),
                 first->get_name().c_str(),
                 color_second.c_str(),
                 second->get_name().c_str(),
                 to_str(op));
        
        log_file << buf.c_str();
    }

    void log_unary_op(const Observed<T> *obj, Operator op) {
        clear_buf();

        std::string color;
        if (obj->is_tmp()) {
            color = '#' + to_string(CLR_VAR_BAD);
        } else {
            color = '#' + to_string(CLR_VAR_NORMAL);
        }

        snprintf(buf.data(), buf.size(), unary_op_node_format.c_str(),
                 ++node_cnt,
                 color.c_str(),
                 obj->get_name().c_str(),
                 to_str(op));

        log_file << buf.c_str();
    }

    void bind_node_ranks(const std::vector<int> &nodes, const std::vector<bool> &is_var_node) {
        std::string bind_command = "{rank = same";

        for (size_t i = 0; i < nodes.size(); ++i) {
            if (i < is_var_node.size() && is_var_node[i]) {
                bind_command += ";node_" + std::to_string(nodes[i]);
            } else {
                bind_command += ";node" + std::to_string(nodes[i]);
            }
        }

        bind_command += "}\n";

        log_file << bind_command;
    }

    void log_operator(const OperatorSignal<T> &signal) {
        const Observed<T> *first = signal.first;
        const Observed<T> *second = signal.second;

        int node_one = first ? get_node(first) : -1;
        int node_two = second ? get_node(second) : -1;

        // if (!node_one && !node_two) {
        //     logger.warning("MicroLoggerGraph", "operator with no operands occured!");
        //     return;
        // }

        if (!second) {
            log_unary_op(first, signal.op);
            int node_cur = node_cnt;

            if (is_creating_op(signal.op)) {
                log_var_node(first);
                draw_arrow_move(node_cur, node_cnt);
                obs_to_node[first] = node_cnt;
            } else if (is_node_replacing_op(signal.op)) {
                if (signal.op != Operator::dtor) {
                    draw_arrow_actor(node_one, node_cur, signal.op);
                    log_var_node(first);
                    draw_arrow_actor(node_cur, node_cnt, signal.op);
                    draw_arrow_move(node_one, node_cnt);
                    obs_to_node[first] = node_cnt;
                } else {
                    draw_arrow_move(node_one, node_cur);
                }
            } else {
                draw_arrow_actor(node_one, node_cur, signal.op);
            }

            // if (is_creating_op(signal.op) || is_node_replacing_op(signal.op)) {
            //     log_var_node(first);
                
            //     if (is_creating_op(signal.op)) {
            //         draw_arrow_move(node_cur, node_cnt);
            //     } else {
            //         draw_arrow_move(obs_to_node[first], node_cur);
            //     }

            //     obs_to_node[first] = node_cnt;
            // }
            
            draw_arrow_flow(node_cur, node_cnt + 1);
        } else {
            log_binary_op(first, second, signal.op);
            int node_cur = node_cnt;

            if (is_creating_op(signal.op)) {
                draw_arrow_actor(node_two, node_cnt, signal.op);
                log_var_node(first);
                obs_to_node[first] = node_cnt;
                node_one = obs_to_node[first];
                draw_arrow_move(node_cur, node_one);
            } else if (is_node_replacing_op(signal.op)) {
                draw_arrow_actor(node_cnt, node_one, signal.op, "", false);
                draw_arrow_actor(node_two, node_cnt, signal.op);
            } else {
                draw_arrow_actor(node_one, node_cnt, signal.op);
                draw_arrow_actor(node_two, node_cnt, signal.op);
            }

            // if (is_creating_op(signal.op) || is_node_replacing_op(signal.op)) {
            //     node_one = obs_to_node[first] = node_cnt;
            // }

            // log_var_node(first);
            // log_var_node(second);

            // bind_node_ranks({node_cur, node_cur + 1, node_cur + 2}, {0, 1, 1});

            draw_arrow_flow(node_cur, node_cnt + 1);
        }
    }
    

    void log_func(const FuncCallSignal &signal, const std::string &color) {
        clear_buf();

        snprintf(buf.data(), buf.size(), func_node_format.c_str(),
                 ++node_cnt,
                 signal.name.c_str(),
                 color.c_str());

        log_file << buf.c_str();
    }

    void log_func(const FuncCallSignal &signal) {
        if (signal.is_called) {
            log_func(signal, "44CC44");
        } else {
            log_func(signal, "4444EE");
        }
        int node_cur = node_cnt;
        draw_arrow_flow(node_cur, node_cnt + 1);
    }

    void info_node(const std::string &label, int node_idx, const std::string &shape="diamond") {
        log_file << "node" << node_idx << "[label=<" + label + "> shape=" + shape + "]\n";
    }

    void log_head() {
        std::string tmp_string;
        std::stringstream buffer;

        std::ifstream tmp_file("formats/head.gv_format");
        buffer << tmp_file.rdbuf();
        log_file << buffer.str();
        tmp_file.close();

        info_node("Start of log", node_cnt, "trapezium");
        draw_arrow_flow(node_cnt, node_cnt + 1);
    }

    void log_tail() {
        info_node("End of log", ++node_cnt, "invtrapezium");
        log_file << "}}"; 
    }

    void graph() {
        if (graphed) {
            logger.error("MicroLoggerGraph", "sorry, can't graph() more then once per logger");
            logger.error("MicroLoggerGraph", "operations are saved in the file, you have to fix it to meet dot format");
            logger.error("MicroLoggerGraph", "(only places between graphs() have to be fixed)");

            log_tail();
            log_file.close();
            log_head();

            return;
        } else {
            graphed = true;
        }

        log_tail();
        log_file.close();

        std::string output_name = file_name + ".svg";
        
        std::vector<char> command_buffer(100);
        snprintf(command_buffer.data(), command_buffer.size(), "dot %s -Tsvg -o%s", file_name.c_str(), output_name.c_str());
        system(command_buffer.data());

        command_buffer.assign(100, 0);
        snprintf(command_buffer.data(), command_buffer.size(), "eog %s", output_name.c_str());
        // system(command_buffer.data());

        log_file.open(file_name, std::ios_base::app);
    }
};
