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


const double GRAPH_RANGE_X = 3000;
const double GRAPH_RANGE_Y = 500000;
const double GRAPH_RANGE_ARROW_COEF = 0.05;


const RColor COORD_LINE_COLOR = {210, 210, 210};

// ==========


class GraphSortingStatistics : public EventReaction<Event::Clicked> {
    sort_func_signature<Observed<int>*> sort_func;
    
    std::vector<std::vector<Observed<int>>> *data;
    size_t tests_cap;

    v_Plottet *asgn_plot;
    v_Plottet *comp_plot;

public:
    GraphSortingStatistics(sort_func_signature<Observed<int>*> sort_func,
                           std::vector<std::vector<Observed<int>>> *data,
                           size_t tests_cap,
                           v_Plottet *asgn_plot,
                           v_Plottet *comp_plot):
    sort_func(sort_func),
    data(data),
    tests_cap(tests_cap),
    asgn_plot(asgn_plot),
    comp_plot(comp_plot)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult * = nullptr) override {
        std::vector<SortStatistics<int>> stats;
        for (size_t i = 0; i < std::min(tests_cap, data->size()); ++i) {
            SortStatistics<int> stat = gather_sort_statistics((*data)[i], sort_func);
            stats.push_back(stat);
        }

        auto graph_asgn = get_graph(stats, SortStatistics<int>::ResultType::asgn);
        auto graph_comp = get_graph(stats, SortStatistics<int>::ResultType::comp);

        {
            PlotterColorSet color_setter1(*asgn_plot, randcolor(125, 200));
            PlotterColorSet color_setter2(*comp_plot, asgn_plot->get_draw_color());

            asgn_plot->draw_graph(graph_asgn, true, false);
            comp_plot->draw_graph(graph_comp, true, false);
        }

        return EventAccResult::none;
    }

    void set_test_data(std::vector<std::vector<Observed<int>>> *new_data) {
        data = new_data;
    }

    std::vector<std::vector<Observed<int>>> *get_data() { return data; }
};


class ClearGraph : public EventReaction<Event::Clicked> {
    v_Plottet *asgn_plot;
    v_Plottet *comp_plot;

public:
    ClearGraph(v_Plottet *asgn_plot,
               v_Plottet *comp_plot):
    asgn_plot(asgn_plot),
    comp_plot(comp_plot)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult * = nullptr) override {
        asgn_plot->clear();
        comp_plot->clear();

        asgn_plot->draw_coord_lines(COORD_LINE_COLOR);
        comp_plot->draw_coord_lines(COORD_LINE_COLOR);

        return EventAccResult::none;
    }
};


v_Window *spawn_new_tests_dw(std::vector<GraphSortingStatistics*> testers);


class RegenerateTests : public EventReaction<Event::Clicked> {
    std::vector<GraphSortingStatistics*> testers;

public:
    RegenerateTests(std::vector<GraphSortingStatistics*> testers):
    testers(testers)
    {}

    ~RegenerateTests() {
    }

    EventAccResult operator()(const Event::Clicked &, const EventAccResult * = nullptr) override {
        if (!testers.size()) return EventAccResult::none;
        
        spawn_new_tests_dw(testers);

        return EventAccResult::none;
    }
};


class NewTestsReaction : public EventReaction<Event::Clicked> {
    int init_size;
    int step;
    double mult;
    int test_cnt;

    std::vector<GraphSortingStatistics*> testers;

public:
    NewTestsReaction(std::vector<GraphSortingStatistics*> testers):
    init_size(1),
    step(50),
    mult(1),
    test_cnt(50),
    testers(testers)
    {}

    int * get_init_size_ptr() { return &init_size; }
    int * get_step_ptr() { return &step; }
    double * get_mult_ptr() { return &mult; }
    int * get_test_cnt_ptr() { return &test_cnt; }

    EventAccResult operator()(const Event::Clicked &, const EventAccResult * = nullptr) override {
        if (!testers.size()) return EventAccResult::none;

        delete testers[0]->get_data();

        printf("%d %g %d %d\n", init_size, mult, step, test_cnt);

        auto data = generate_tests<int>(step, mult, init_size, test_cnt);

        for (size_t i = 0; i < testers.size(); ++i) {
            testers[i]->set_test_data(data);
        }

        return EventAccResult::none;
    }
};


v_Window *spawn_new_tests_dw(std::vector<GraphSortingStatistics*> testers) {
    auto dw = new v_DialogWindow("New canvas", 350, 5, 0);
    
    auto f_init_size = dw->add_field("Init size", 50);
    auto f_step      = dw->add_field("Step", 50);
    auto f_mult      = dw->add_field("Mult", 50);
    auto f_test_cnt  = dw->add_field("Test count", 50);

    auto b_create = dw->add_accept_button("Create");

    auto reaction = new NewTestsReaction(testers);
    b_create->e_clicked.add(reaction);

    f_init_size->e_text_changed.add (new TextFieldChangeValueSynchronizer(reaction->get_init_size_ptr()));
    f_step->e_text_changed.add(new TextFieldChangeValueSynchronizer(reaction->get_step_ptr()));
    f_mult->e_text_changed.add  (new TextFieldChangeValueSynchronizer(reaction->get_mult_ptr()));
    f_test_cnt->e_text_changed.add  (new TextFieldChangeValueSynchronizer(reaction->get_test_cnt_ptr()));

    dw->make_closing_field(f_init_size, b_create);
    dw->make_closing_field(f_step, b_create);
    dw->make_closing_field(f_mult, b_create);
    dw->make_closing_field(f_test_cnt, b_create);
    dw->make_closing_button(b_create);

    f_init_size->set_string("1");
    f_step->set_string("50");
    f_mult->set_string("1");
    f_test_cnt->set_string("50");

    App.app_engine->add_view(dw);

    dw->select_first_field();

    return dw;
}


double parabola(double x) {
    return x * x;
}


double nlogn(double x) {
    return x * log(x);
}


void func(const OperatorSignal<int> &sig) {
    printf("operator[%s] on [%d] and [%d]\n", to_str(sig.op), sig.first->get_id(), sig.second->get_id());
}


void initialize_photoshop(RedactorEngine &moga) {
    auto asgn_plotter = new v_Plottet({SCR_D, {(SCR_W / 2 - SCR_D * 4), SCR_H - SCR_D * 2}}, {70, 70, 70}, 
                                      Range2d{
                                          {- GRAPH_RANGE_X * GRAPH_RANGE_ARROW_COEF, - GRAPH_RANGE_Y * GRAPH_RANGE_ARROW_COEF},
                                          {GRAPH_RANGE_X, GRAPH_RANGE_Y}
                                      }
                                     );
    
    auto comp_plotter = new v_Plottet({{(SCR_W / 2 + SCR_D * 3), SCR_D}, {(SCR_W / 2 - SCR_D * 4), SCR_H - SCR_D * 2}}, {70, 70, 70}, 
                                      Range2d{
                                          {- GRAPH_RANGE_X * GRAPH_RANGE_ARROW_COEF, - GRAPH_RANGE_Y * GRAPH_RANGE_ARROW_COEF},
                                          {GRAPH_RANGE_X, GRAPH_RANGE_Y}
                                      }
                                     );
    
    asgn_plotter->draw_coord_lines(COORD_LINE_COLOR);
    comp_plotter->draw_coord_lines(COORD_LINE_COLOR);

    moga.add_view(asgn_plotter);
    moga.add_view(comp_plotter);

    auto asgn_label = new v_Highlighter({{asgn_plotter->get_body().position.x(), 0}, {asgn_plotter->get_body().size.x(), SCR_D}});
    auto comp_label = new v_Highlighter({{comp_plotter->get_body().position.x(), 0}, {comp_plotter->get_body().size.x(), SCR_D}});

    asgn_label->add_label("Assignments", SCR_D - 10, {225, 10, 10});
    comp_label->add_label("Comparisons", SCR_D - 10, {225, 10, 10});

    moga.add_view(asgn_label);
    moga.add_view(comp_label);

    // ==================================================================================

    auto test_data = generate_tests<int>(50, 1, 10, 50);

    // ==================================================================================

    auto dw = new v_DialogWindow("Bububu", 250);
    
    auto bubble    = dw->add_text_button("Bubble",     true);
    auto insertion = dw->add_text_button("Insertions", true);
    auto selection = dw->add_text_button("Selections", true);
    auto std_sort  = dw->add_text_button("StdSort",  true);
    auto merge     = dw->add_text_button("Mergesort",  true);

    auto clear = dw->add_text_button("Clear", true);
    auto regen_tests = dw->add_text_button("New tests", true);


    auto bubble_tester    = new GraphSortingStatistics(bubble_sort,     test_data,  30, asgn_plotter, comp_plotter);
    auto insertion_tester = new GraphSortingStatistics(insertions_sort, test_data,  30, asgn_plotter, comp_plotter);
    auto selection_tester = new GraphSortingStatistics(selection_sort,  test_data,  30, asgn_plotter, comp_plotter);
    auto std_sort_tester  = new GraphSortingStatistics(std::sort,       test_data, 100, asgn_plotter, comp_plotter);
    auto merge_tester     = new GraphSortingStatistics(merge_sort,      test_data, 100, asgn_plotter, comp_plotter);

    bubble   ->e_clicked.add(bubble_tester);
    insertion->e_clicked.add(insertion_tester);
    selection->e_clicked.add(selection_tester);
    std_sort ->e_clicked.add(std_sort_tester);
    merge    ->e_clicked.add(merge_tester);

    clear->e_clicked.add(new ClearGraph(asgn_plotter, comp_plotter));

    regen_tests->e_clicked.add(new RegenerateTests({bubble_tester, insertion_tester, selection_tester, std_sort_tester, merge_tester}));

    moga.add_view(dw);

    // ==================================================================================

    
    // plotter->draw_graph(parabola);
    // plotter->draw_graph(nlogn);
}
