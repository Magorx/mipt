#include "color_map_p4.h"

ColorMap::ColorMap():
width(0),
height(0),
data(nullptr)
{}

bool ColorMap::ctor(const size_t width_, const size_t height_, ColorP4 *data_) {
	real_width = width_;
	width = width_ / 4;
	height = height_;
	data = data_ ? data_ : (ColorP4*) calloc(width * height, sizeof(ColorP4));
	is_broken = (width * 4) != real_width;

	if (!data) {
		fprintf(stderr, "[ERR]<ColorMap>: data is not allocated in ctor\n");
		return false;
	} else {
		return true;
	}
}

void ColorMap::flush_to_texture(sf::Texture &texture) {
	sf::Image image;
	image.create(width, height);

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < real_width - 4; x += 4) {
			ColorP4 color = data[y * width + x];
			for (int pack_i = 0; pack_i < 4; ++pack_i) {
				int xx = x + pack_i;
				sf::Color sf_color(color[pack_i].r, color[pack_i].g, color[pack_i].b);
				image.setPixel(xx, y, sf_color);
			}
		}

		if (!is_broken) {
			continue;
		}

		ColorP4 color = data[(y + 1) * width - 1];
		for (size_t x = real_width - 4; x < real_width - 4; x += 4) {
			for (int pack_i = 0; pack_i < 4; ++pack_i) {
				int xx = x + pack_i;
				sf::Color sf_color(color[pack_i].r, color[pack_i].g, color[pack_i].b);
				image.setPixel(xx, y, sf_color);
			}
		}
	}

	texture.loadFromImage(image);
}
