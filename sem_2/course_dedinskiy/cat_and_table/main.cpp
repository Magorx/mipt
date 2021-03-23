#include <cstdlib>
#include <cstdio>

#include "color_map_p4.h"

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

	ColorMapP4 cmap = {};
	cmap.ctor(SCR_W, SCR_H);
	for (size_t i = 0; i < cmap.height; ++i) {
		for (size_t j = 0; j < cmap.width; ++j) {
			cmap[i][j] = ARGB(255 * i / cmap.height, 255 * j / cmap.width, 125);
		}
	}

	ColorMapP4 cm = {};
	cm.ctor(SCR_W, SCR_H);
	for (size_t i = 0; i < cm.height; ++i) {
		for (size_t j = 0; j < cm.width; ++j) {
			cm[i][j] = ARGB(120 * (j % 2), 255 * (j % 2) - 255 * j / cm.width, 255 - 255 * 255 * j * i / cm.width / cm.height, 255.0f / cm.width * j);
		}
	}

	cmap.superimpose_alpha(cm, 0, 0);

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