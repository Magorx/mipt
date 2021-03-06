#include <SFML/Graphics.hpp>
#include "general/general_c.h"
#include "color_map_p4.h"
#include "mandelbroter.h"

enum MODES {
    TIMETEST_MODE,
    REAL_MODE
};
const int MODE = REAL_MODE;

const float SCR_RATIO = 16 / 9;

const int SCR_H = 300;
const int SCR_W = 300;

const float INIT_H = 2;
const float INIT_W = INIT_H * SCR_RATIO;

const float STEP_SCALE  = 0.25;
const float SCALE_SCALE = 0.75;

int main()
{
    sf::Texture texture;
    texture.create(SCR_W, SCR_H);
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // ============

    ColorMapP4 cmap;
    cmap.ctor(SCR_W, SCR_H);

    float cur_scale = 1;
    float cur_x = 0;
    float cur_y = 0;

    if (MODE == TIMETEST_MODE) {
        int test_itters = 100;
        printf("tes_itters: %d\n", test_itters);
        
        printf("Testing ussual render ---\n");
        TIMER_START();
        for (int i = 0; i < test_itters; ++i) {
            render_mandelbrot(&cmap, cur_x, cur_y, INIT_W * cur_scale, INIT_H * cur_scale);
        }
        TIMER_END_AND_PRINT();

        printf("Testing v_float_4 render ---\n");
        TIMER_START();
        for (int i = 0; i < test_itters; ++i) {
            render_mandelbrot_v_float_4(&cmap, cur_x, cur_y, INIT_W * cur_scale, INIT_H * cur_scale);
        }
        TIMER_END_AND_PRINT();

        printf("Testing intrinsic render ---\n");
        TIMER_START();
        for (int i = 0; i < test_itters; ++i) {
            render_mandelbrot_intrinsics(&cmap, cur_x, cur_y, INIT_W * cur_scale, INIT_H * cur_scale);
        }
        TIMER_END_AND_PRINT();

    } else {
        int render_option = 0;

        sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), "Mandaloter");
        while (window.isOpen())
        {
            TIMER_START();
            if (render_option == 0) {
                render_mandelbrot_intrinsics(&cmap, cur_x, cur_y, INIT_W * cur_scale, INIT_H * cur_scale);
            } else {
                render_mandelbrot(&cmap, cur_x, cur_y, INIT_W * cur_scale, INIT_H * cur_scale);
            }
            TIMER_END();

            printf("[FPS]: %lg\n", 1 / GLOBAL_TIMER_INTERVAL);

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::W) {
                        cur_y += -1 * cur_scale * STEP_SCALE;
                    }
                    if (event.key.code == sf::Keyboard::S) {
                        cur_y += +1 * cur_scale * STEP_SCALE;
                    }
                    if (event.key.code == sf::Keyboard::A) {
                        cur_x += -1 * cur_scale * STEP_SCALE;
                    }
                    if (event.key.code == sf::Keyboard::D) {
                        cur_x += +1 * cur_scale * STEP_SCALE;
                    }
                    if (event.key.code == sf::Keyboard::E) {
                        cur_scale *= SCALE_SCALE;
                        if (cur_scale < 0.000000001) {
                            cur_scale = 1000;
                        }
                    }
                    if (event.key.code == sf::Keyboard::Q) {
                        cur_scale /= SCALE_SCALE;
                        if (cur_scale > 1000) {
                            cur_scale = 0.000000001;
                        }
                    }
                    if (event.key.code == sf::Keyboard::Z) {
                        render_option ^= 1;
                    }
                    if (event.key.code == sf::Keyboard::F) {
                        texture.copyToImage().saveToFile("scr.png");
                    }
                }
            }

            window.clear();
            cmap.flush_to_texture(texture);
            window.draw(sprite);
            window.display();
        }
    }

    return 0;
}
