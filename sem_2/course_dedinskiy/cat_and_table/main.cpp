#include <cstdlib>
#include <cstdio>

#include "color_map_p4.h"

const double SCR_RATIO = 9.0 / 16.0;

const size_t   SCR_W = 800;
const size_t   SCR_H = SCR_W * SCR_RATIO;

int main() {
	printf("%lu %lu\n", sizeof(ColorP4), sizeof(ARGB));
	// ===========================
	sf::Texture texture;
    texture.create(SCR_W, SCR_H);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), "Catable");
    // ===========================

	ColorMapP4 cmap = {};
	cmap.ctor(SCR_W, SCR_H);

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