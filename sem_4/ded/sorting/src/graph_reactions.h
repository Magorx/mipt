class GraphSortingStatistics : public EventReaction<Event::Clicked> {
    sort_func_signature<Observed<int>*> sort_func;
    
    std::vector<std::vector<Observed<int>>> *data;

    v_Plottet *asgn_plot;
    v_Plottet *comp_plot;

    RColor color;

public:
    GraphSortingStatistics(sort_func_signature<Observed<int>*> sort_func,
                           std::vector<std::vector<Observed<int>>> *data,
                           v_Plottet *asgn_plot,
                           v_Plottet *comp_plot,
                           RColor color):
    sort_func(sort_func),
    data(data),
    asgn_plot(asgn_plot),
    comp_plot(comp_plot),
    color(color)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult * = nullptr) override {
        std::vector<SortStatistics<int>> stats;
        for (size_t i = 0; i < data->size(); ++i) {
            SortStatistics<int> stat = gather_sort_statistics((*data)[i], sort_func);
            stats.push_back(stat);

            if (stat.asgn_cnt > GRAPH_RANGE_Y || stat.comp_cnt > GRAPH_RANGE_Y) {
                break;
            }
        }

        auto graph_asgn = get_graph(stats, SortStatistics<int>::ResultType::asgn);
        auto graph_comp = get_graph(stats, SortStatistics<int>::ResultType::comp);

        {
            PlotterColorSet tmp_color_asgn(*asgn_plot, color);
            PlotterColorSet tmp_color_comp(*comp_plot, color);

            asgn_plot->draw_graph(graph_asgn, TO_SHOW_DOTS, TO_SHOW_COLUMNS);
            comp_plot->draw_graph(graph_comp, TO_SHOW_DOTS, TO_SHOW_COLUMNS);
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

        asgn_plot->draw_coord_checkers(0.05, COORD_LINE_COLOR / 2);
        comp_plot->draw_coord_checkers(0.05, COORD_LINE_COLOR / 2);

        asgn_plot->draw_coord_lines(COORD_LINE_COLOR);
        comp_plot->draw_coord_lines(COORD_LINE_COLOR);

        return EventAccResult::none;
    }
};


class DrawFunc : public EventReaction<Event::Clicked> {
    v_Plottet *asgn_plot;
    v_Plottet *comp_plot;
    double (*func)(double);

public:
    DrawFunc(v_Plottet *asgn_plot,
             v_Plottet *comp_plot,
             double (*func)(double)):
    asgn_plot(asgn_plot),
    comp_plot(comp_plot),
    func(func)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult * = nullptr) override {
        PlotterColorSet tmp_color_asgn(*asgn_plot, NORMAL_GRAPHICS_COLOR);
        PlotterColorSet tmp_color_comp(*comp_plot, NORMAL_GRAPHICS_COLOR);

        asgn_plot->draw_graph(func, 10, 1);
        comp_plot->draw_graph(func, 10, 1);

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
    auto dw = new v_DialogWindow("New tests", 350, 5, 0);
    
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
    f_test_cnt->set_string("100");

    App.app_engine->add_view(dw);

    dw->select_first_field();

    return dw;
}