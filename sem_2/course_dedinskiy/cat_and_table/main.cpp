#include <cstdlib>
#include <cstdio>

#include "color_map_p4.h"
#include "general/c/timer.h"

const double SCR_RATIO = 9.0 / 16.0;

const size_t   SCR_W = 800;
const size_t   SCR_H = SCR_W * SCR_RATIO;

int main() {
	// ===========================
	sf::Texture texture;
    texture.create(SCR_W, SCR_H);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), "Catable");
    // ===========================

 //    ColorMapP4 cm = {};
	// cm.ctor(SCR_W, SCR_H);
	// for (size_t i = 0; i < cm.height; ++i) {
	// 	for (size_t j = 0; j < cm.width; ++j) {
	// 		cm[i][j] = ARGB(120, 255 * j / cm.width, 255 - 255 * 255 * j * i / cm.width / cm.height, 100);
	// 	}
	// }

	ColorMapP4 cmap = {};
	cmap.ctor("Table.bmp");

	ColorMapP4 cm = {};
	cm.ctor("AskhatCat.bmp");

	// printf("normal\n");

	// TIMER_START();
	// for (int i = 0; i < 100000; ++i) {
	// 	cmap.superimpose_alpha(cm, cmap.real_width * 0, cmap.height * 0);
	// }
	// TIMER_END_AND_PRINT();

	// printf("fast\n");

	// TIMER_START();
	// for (int i = 0; i < 100000; ++i) {
		cmap.superimpose_alpha_intr(cm, cmap.real_width * 0.35, cmap.height * 0.35);
	// }
	// TIMER_END_AND_PRINT();


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        cmap.flush_to_texture(texture);
        window.draw(sprite);
        window.display();
    }

	printf("doned\n");
	return 0;
}