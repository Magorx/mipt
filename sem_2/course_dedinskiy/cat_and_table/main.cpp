#include <cstdlib>
#include <cstdio>

#include "color_map_p4.h"
#include "general/c/timer.h"


const int RUN_GRAD = 1;
const int RUN_CAT  = 2;
const int TEST = 0;
const int MODE = TEST;

const double SCR_RATIO = 9.0 / 16.0;

const size_t   SCR_W = 800;
const size_t   SCR_H = SCR_W * SCR_RATIO;

int main() {
	// ===========================
	if (MODE != TEST) {
		sf::Texture texture;
	    texture.create(SCR_W, SCR_H);
	    sf::Sprite sprite;
	    sprite.setTexture(texture);
	    sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), "Catable");

	    ColorMapP4 cmap = {};
		cmap.ctor("Table.bmp");

		ColorMapP4 cm = {};

		if (MODE == RUN_GRAD) {
			cm.ctor(SCR_W, SCR_H);
			for (size_t i = 0; i < cm.height; ++i) {
				for (size_t j = 0; j < cm.width; ++j) {
					cm[i][j] = ARGB(255 - 255.0f / cm.width * j, 255 * j / cm.width, 255 - 255 * i / cm.height, 255.0f / cm.width * j);
				}
			}

			cmap.superimpose_alpha_intr(cm, cmap.real_width * 0, cmap.height * 0);
		} else {
			cm.ctor("AskhatCat.bmp");
			cmap.superimpose_alpha_intr(cm, cmap.real_width * 0.35, cmap.height * 0.35);
		}

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

	    cmap.detr();
	    cm.detr();
	} else {
		ColorMapP4 cm = {};
		cm.ctor(SCR_W, SCR_H);
		for (size_t i = 0; i < cm.height; ++i) {
			for (size_t j = 0; j < cm.width; ++j) {
				cm[i][j] = ARGB(120, 255 * j / cm.width, 255 - 255 * 255 * j * i / cm.width / cm.height, 255.0f / cm.width / j);
			}
		}

		ColorMapP4 cmap = {};
		cmap.ctor("Table.bmp");

		// ColorMapP4 cm = {};
		// cm.ctor("AskhatCat.bmp");

		const int iters_cnt = 1000;
		printf("iters_cnt: %d\n", iters_cnt);

		printf("[sse]<of>:\n");
		TIMER_START();
		for (int i = 0; i < iters_cnt; ++i) {
			cmap.superimpose_alpha(cm, cmap.real_width * 0, cmap.height * 0);
		}
		TIMER_END_AND_PRINT();

		printf("[sse]<on>:\n");
		TIMER_START();
		for (int i = 0; i < iters_cnt; ++i) {
			cmap.superimpose_alpha_intr(cm, cmap.real_width * 0, cmap.height * 0);
		}
		TIMER_END_AND_PRINT();

		cmap.detr();
	    cm.detr();
	}

	printf("doned\n");
	return 0;
}