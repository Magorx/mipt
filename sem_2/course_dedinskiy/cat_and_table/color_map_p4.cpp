#include "color_map_p4.h"

ColorMapP4::ColorMapP4():
width(0),
height(0),
data(nullptr)
{}

bool ColorMapP4::ctor(const size_t width_, const size_t height_, ColorP4 *data_) {
	real_width = width_;
	width = width_ / 4;
	height = height_;
	data = data_ ? data_ : (ColorP4*) calloc(real_width * height, sizeof(ARGB));
	is_broken = (width * 4) != real_width;

	if (!data) {
		fprintf(stderr, "[ERR]<ColorMapP4>: data is not allocated in ctor\n");
		return false;
	} else {
		return true;
	}
}

void ColorMapP4::flush_to_texture(sf::Texture &texture) {
	sf::Image image;
	image.create(real_width, height);

	ARGB *cdata = (ARGB*) data;

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < real_width; ++x) {
			ARGB color = cdata[y * real_width + x];
			sf::Color sf_color(color.r, color.g, color.b);
			image.setPixel(x, y, sf_color);
		}
	}

	texture.loadFromImage(image);
}

ARGB *ColorMapP4::operator[](const size_t i) {
	return ((ARGB*) data) + i * real_width;
}
