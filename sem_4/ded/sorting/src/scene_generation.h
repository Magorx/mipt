void create_cage(MogaEngine *eng, SmartColor *color) {
    o_Line *line_1 = new o_Line({100, 100, 0}, 
                                {350, 0, 0}, 
                                color);

    o_Line *line_11 = new o_Line({350, 0, 0}, 
                                 {700, 100, 0}, 
                                 color);
    
    o_Line *line_2 = new o_Line({100, 100, 0}, 
                                {100, 500, 0}, 
                                color);
    
    o_Line *line_3 = new o_Line({700, 100, 0}, 
                                {700, 500, 0},
                                color);
    
    o_Line *line_4 = new o_Line({100, 500, 0}, 
                                {700, 500, 0}, 
                                color);

    eng->add_object(line_1);
    eng->add_object(line_2);
    eng->add_object(line_3);
    eng->add_object(line_4);
    eng->add_object(line_11);
}

void gen_ball(MogaEngine *eng) {
    Color col = Vec3d::random_unit() * 250;
    col.content[3] = 255;
    SmartColor *color = nullptr;
    color = new SmartColor(col);
    
    Object *ball = new o_Ball({(double) (rand() % 500) + 150, (double)(rand() % 300) + 120, 0}, 5, color, 1);
    eng->add_object(ball);
    eng->add_tickable(color);

    Vec3d vel = Vec3d::random_unit() * 200;
    vel.set(2, 0);
    ball->get_solid_body()->set_velocity(vel);
}

void generate_balls(MogaEngine *eng, int cnt = 100) {
    for (int i = 0; i < cnt; ++i) {
        gen_ball(eng);
    }

    auto color = new SmartColorSin(Color(255, 255, 255), 2);
    eng->add_tickable(color);

    Object *ball = new o_Ball({500, 300, 0}, 30, color, 6);
    eng->add_object(ball);
}