#pragma once


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

    std::ofstream log_file;
    std::string file_name;

    std::string var_node_format;
    std::string unary_op_node_format;
    std::string binary_op_node_format;

    std::string buf;

    int node_cnt = 0;

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

        log_head();

        buf.resize(1024);
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
        snprintf(buf.data(), buf.size(), "node%d->node%d [style=dashed, constraint=true, color=darkmagenta];\n",
                 node_from,
                 node_to);
        log_file << buf.c_str();
    }

    void draw_arrow_flow(int node_from, int node_to) {
        clear_buf();
        snprintf(buf.data(), buf.size(), "node%d->node%d [constraint=true, color=crimson];\n",
                 node_from,
                 node_to);
        log_file << buf.c_str();
    }

    void log_var_node(const Observed<T> *obj) {
        clear_buf();

        snprintf(buf.data(), buf.size(), var_node_format.c_str(),
                 ++node_cnt,
                 obj->get_name().c_str(), obj->get_id(),
                 obj,
                 obj->to_str().c_str());

        log_file << buf.c_str();
    }
    
    void log_binary_op(const Observed<T> *first, const Observed<T> *second, Operator op) {
        clear_buf();

        snprintf(buf.data(), buf.size(), binary_op_node_format.c_str(),
                 ++node_cnt,
                 first->get_name().c_str(),
                 first->get_id(),
                 second->get_name().c_str(),
                 second->get_id(),
                 to_str(op));
        
        log_file << buf.c_str();
    }

    void log_unary_op(const Observed<T> *obj, Operator op) {
        clear_buf();

        snprintf(buf.data(), buf.size(), unary_op_node_format.c_str(),
                 ++node_cnt,
                 obj->get_name().c_str(),
                 obj->get_id(),
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

        if (!node_one && !node_two) {
            logger.warning("MicroLoggerGraph", "operator with no operands occured!");
            return;
        }

        if (!second) {
            log_unary_op(first, signal.op);
            int node_cur = node_cnt;
            
            if (!is_creating_op(signal.op)) {
                draw_arrow_move(node_one, node_cur);
            }

            log_var_node(first);

            bind_node_ranks({node_cur, node_cnt}, {0, 1});

            if (is_creating_op(signal.op) || is_node_replacing_op(signal.op)) {
                obs_to_node[first] = node_cur;
            }
            
            draw_arrow_flow(node_cur, node_cnt + 1);
        } else {
            log_binary_op(first, second, signal.op);
            int node_cur = node_cnt;

            if (!is_creating_op(signal.op)) {
                draw_arrow_move(node_one, node_cnt);
                draw_arrow_move(node_two, node_cnt);
            }

            if (is_creating_op(signal.op) || is_node_replacing_op(signal.op)) {
                node_one = obs_to_node[first] = node_cnt;
            }

            log_var_node(first);
            log_var_node(second);

            bind_node_ranks({node_cur, node_cur + 1, node_cur + 2}, {0, 1, 1});

            draw_arrow_flow(node_cur, node_cnt + 1);
        }
    }

    void log_func(const FuncCallSignal &) {
        
    }

    void log_head() {
        std::string tmp_string;
        std::stringstream buffer;

        std::ifstream tmp_file("formats/head.gv_format");
        buffer << tmp_file.rdbuf();
        log_file << buffer.str();
        tmp_file.close();
    }

    void log_tail() {
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