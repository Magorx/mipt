#include <SFML/Graphics.hpp>
#include <cmath>

#include "sfml_extension.h"

#include "showcase_funcs.h"

const size_t SCR_W = 1200;
const size_t SCR_H = 800; 

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    CoordinateSystem gr_parabosina({40, 40}, 150, 150);
    gr_parabosina.set_ranges(-15, 15, -15, 15);
    gr_parabosina.set_angle(15);
    gr_parabosina.set_window(&window);
    gr_parabosina.set_fill_color({30, 30, 30});
    gr_parabosina.set_draw_color({200, 100, 50});

    CoordinateSystem gr_sin({650, 120}, 500, 200);
    gr_sin.set_ranges(-15, 15, -1, 1);
    gr_sin.set_angle(-10);
    gr_sin.set_window(&window);

    CoordinateSystem gr_pfunc({450, 450}, 700, 300);
    gr_pfunc.set_ranges(-1, 8, -5, 5);
    gr_pfunc.set_window(&window);

    CoordinateSystem gr_spiral({0, 0}, SCR_W, SCR_H);
    gr_spiral.set_ranges(-100, 100, -100, 100);
    gr_spiral.set_angle(0);
    gr_spiral.set_window(&window);
    gr_spiral.set_fill_color({30, 30, 30});
    gr_spiral.set_draw_color({200, 200, 200});

    int timer = time(NULL);
    int last_timer = timer;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        timer = time(NULL);
        if (timer > last_timer + 1) {
            last_timer = timer;

            float x = rand() % SCR_W;
            float y = rand() % SCR_H;

            gr_parabosina.set_bias({x, y});
        }

        // ====================================================================
        gr_spiral.graphicate_parametric(&param_spiral_x, &param_spiral_y, &param_color, -10, 10, 10000); //background

        gr_parabosina.graphicate(&parabosina);

        gr_sin.graphicate(&sinf);
        gr_pfunc.graphicate_parametric(&param_x, &param_y, &param_color, -2, 2, 500);

        gr_parabosina.rotate(0.1);
        gr_sin.rotate(-0.2);
        // ====================================================================

        //window.draw(shape);
        window.display();
    }

    return 0;
}
