#include <SFML/Graphics.hpp>
#include <cmath>

#include "sfml_extension.h"
#include "showcase_funcs.h"

const size_t SCR_W = 1200;
const size_t SCR_H = 800;

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), "*~ Harry Plotter ~*");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    CoordinateSystem gr_parabosina({40, 40}, 150, 150,
                                   &window,
                                   Range2f{-15, 15, -15, 15},
                                   15);
    gr_parabosina.set_fill_color({ 30,  30, 30});
    gr_parabosina.set_draw_color({200, 100, 50});

    CoordinateSystem gr_sin({850, 140}, 500, 200,
                            &window,
                            Range2f{-15, 15, -1, 1},
                            -10);

    CoordinateSystem gr_pfunc({850, 650}, 700, 300,
                              &window,
                              Range2f{-1, 8, -5, 5});

    CoordinateSystem gr_spiral({SCR_W / 2, SCR_H / 2}, SCR_W, SCR_H,
                               &window,
                               Range2f{-100, 100, -100, 100});
    gr_spiral.set_fill_color({ 30,  30,  30});
    gr_spiral.set_draw_color({200, 200, 200});

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // ====================================================================
        gr_spiral.graphicate_parametric(&param_spiral_x, &param_spiral_y, &spiral_color, -10, 10, 10000); //background

        for (float i = 0; i < 13; ++i) {
            float coef = 0.37;
            float timer = fmod(1.5 * clock.getElapsedTime().asSeconds(), 2);
            float t = timer + i;
            float x = param_spiral_x(t * coef);
            float y = param_spiral_y(t * coef);
            gr_spiral.set_draw_color(spiral_color(t / 3));
            gr_spiral.draw_vector({0, 0}, {x, y}, M_PI / 8, 0.2);
        }
        gr_spiral.set_draw_color({200, 200, 200});
        gr_spiral.flush();

        gr_sin.graphicate(&sinf);
        gr_pfunc.graphicate_parametric(&param_x, &param_y, &param_color, -2, 2, 500);

        sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
        gr_parabosina.set_position({mouse_pos.x, mouse_pos.y});
        gr_parabosina.graphicate(&parabosina);

        gr_parabosina.rotate(0.1);
        gr_sin.rotate(-0.17);
        // ====================================================================

        window.display();
    }

    return 0;
}
