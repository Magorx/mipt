#pragma once

const int SCR_W  = 1600;
const int SCR_H  = 800;
const int SCR_D  = 50;

#include "redactor/engine.h"
#include "scene_generation.h"
#include "responses.h"

// == task ==

#include "observer/observed.h"
#include "observer/sort/sort.h"

#include <algorithm>
#include <string>


const double GRAPH_RANGE_X = 3000;
const double GRAPH_RANGE_Y = 500000;
const double GRAPH_RANGE_ARROW_COEF = 0.05;


const RColor COORD_LINE_COLOR = {210, 210, 210};

const double QUADRATIC_SORT_TESTS_CNT_COEF  = 0.5;
const double LINEAR_SORT_TESTS_CNT_COEF     = 1.0;

const int CLR_A = 250;
const int CLR_B = 180;
const int CLR_C = 130;


const std::string ASGN_LABEL_NAME = "Assignments";
const std::string COMP_LABEL_NAME = "Comparisons";
const RColor LABEL_COLOR = {225, 10, 10};

const RColor NORMAL_GRAPHICS_COLOR = {10, 255, 10};


const bool TO_SHOW_DOTS = true;
const bool TO_SHOW_COLUMNS = true;


const Range2d DEFAULT_RANGE = Range2d {
    {- GRAPH_RANGE_X * GRAPH_RANGE_ARROW_COEF, - GRAPH_RANGE_Y * GRAPH_RANGE_ARROW_COEF},
    {GRAPH_RANGE_X, GRAPH_RANGE_Y}
};

const Range2d POSITIVE_RANGE = Range2d {
    {std::max(0.0, DEFAULT_RANGE.x_min), std::max(0.0, DEFAULT_RANGE.y_min)},
    {std::max(0.0, DEFAULT_RANGE.x_max), std::max(0.0, DEFAULT_RANGE.y_max)}
};

// ==========


#include "graph_reactions.h"


void log_op_signals(const OperatorSignal<int> &sig) {
    int first_id = sig.first ? sig.first->get_id() : -1;
    int second_id = sig.second ? sig.second->get_id() : -1;
    logger.info("observer", "op [%+9s] | [%7d] | [%7d]", to_str(sig.op), first_id, second_id);
    // printf("operator[%s] on [%d] and [%d]\n", to_str(sig.op), sig.first->get_id(), sig.second->get_id());
}


void initialize_photoshop(RedactorEngine &moga) {
    auto asgn_plotter = new v_Plottet({SCR_D, {(SCR_W / 2 - SCR_D * 4), SCR_H - SCR_D * 2}}, {70, 70, 70}, DEFAULT_RANGE);
    auto comp_plotter = new v_Plottet({{(SCR_W / 2 + SCR_D * 3), SCR_D}, {(SCR_W / 2 - SCR_D * 4), SCR_H - SCR_D * 2}}, {70, 70, 70}, DEFAULT_RANGE);

    asgn_plotter->draw_coord_checkers(0.05, COORD_LINE_COLOR / 2);
    comp_plotter->draw_coord_checkers(0.05, COORD_LINE_COLOR / 2);
    
    asgn_plotter->draw_coord_lines(COORD_LINE_COLOR);
    comp_plotter->draw_coord_lines(COORD_LINE_COLOR);

    moga.add_view(asgn_plotter);
    moga.add_view(comp_plotter);

    auto asgn_label = new v_Highlighter({{asgn_plotter->get_body().position.x(), 0}, {asgn_plotter->get_body().size.x(), SCR_D}});
    auto comp_label = new v_Highlighter({{comp_plotter->get_body().position.x(), 0}, {comp_plotter->get_body().size.x(), SCR_D}});

    std::string asgn_text = ASGN_LABEL_NAME + " [" + std::to_string((int) GRAPH_RANGE_X) + "x" + std::to_string((int) GRAPH_RANGE_Y) + "]";
    std::string comp_text = COMP_LABEL_NAME + " [" + std::to_string((int) GRAPH_RANGE_X) + "x" + std::to_string((int) GRAPH_RANGE_Y) + "]";

    asgn_label->add_label(asgn_text.c_str(), SCR_D - 10, LABEL_COLOR);  
    comp_label->add_label(comp_text.c_str(), SCR_D - 10, LABEL_COLOR);

    moga.add_view(asgn_label);
    moga.add_view(comp_label);

    // ==================================================================================

    auto test_data = generate_tests<int>(50, 1, 10, 100);

    // ==================================================================================

    auto dw = new v_DialogWindow("Bububu", 250);
    
    auto bubble    = dw->add_text_button("Bubble",     true);
    auto insertion = dw->add_text_button("Insertions", true);
    auto selection = dw->add_text_button("Selections", true);
    auto std_sort  = dw->add_text_button("StdSort",    true);
    auto merge     = dw->add_text_button("Mergesort",  true);

    auto gr_quadratic  = dw->add_text_button("N^2", true);
    auto gr_nlogn  = dw->add_text_button("NlogN", true);
    auto gr_n  = dw->add_text_button("N", true);

    auto clear = dw->add_text_button("Clear", true);
    auto regen_tests = dw->add_text_button("New tests", true);

    auto bubble_tester    = new GraphSortingStatistics(bubble_sort,     test_data, asgn_plotter, comp_plotter, {CLR_A, CLR_B, CLR_C});
    auto insertion_tester = new GraphSortingStatistics(insertions_sort, test_data, asgn_plotter, comp_plotter, {CLR_A, CLR_C, CLR_B});
    auto selection_tester = new GraphSortingStatistics(selection_sort,  test_data, asgn_plotter, comp_plotter, {CLR_B, CLR_A, CLR_A});
    auto std_sort_tester  = new GraphSortingStatistics(std::sort,       test_data, asgn_plotter, comp_plotter, {CLR_B, CLR_C, CLR_A});
    auto merge_tester     = new GraphSortingStatistics(merge_sort,      test_data, asgn_plotter, comp_plotter, {CLR_C, CLR_A, CLR_B});

    bubble   ->e_clicked.add(bubble_tester);
    insertion->e_clicked.add(insertion_tester);
    selection->e_clicked.add(selection_tester);
    std_sort ->e_clicked.add(std_sort_tester);
    merge    ->e_clicked.add(merge_tester);

    gr_quadratic->e_clicked.add(new DrawFunc(asgn_plotter, comp_plotter, [](double x) -> double { return x * x; }));
    gr_nlogn->e_clicked.add(new DrawFunc(asgn_plotter, comp_plotter, [](double x) -> double { return x * log(x); }));
    gr_n->e_clicked.add(new DrawFunc(asgn_plotter, comp_plotter, [](double x) -> double { return x; }));

    clear->e_clicked.add(new ClearGraph(asgn_plotter, comp_plotter));

    regen_tests->e_clicked.add(new RegenerateTests({bubble_tester, insertion_tester, selection_tester, std_sort_tester, merge_tester}));

    moga.add_view(dw);

    dw->get_body().position.content[1] = SCR_D * 3;

    // ==================================================================================

}
