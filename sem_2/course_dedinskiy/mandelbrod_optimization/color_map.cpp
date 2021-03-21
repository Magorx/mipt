#include "color_map.h"

ColorMap::ColorMap():
width(0),
height(0),
data(nullptr)
{}

bool ColorMap::ctor(const size_t width_, const size_t height_, Color *data_) {
	width = width_;
	height = height_;
	data = data_ ? data_ : (Color*) calloc(width_ * height_, sizeof(Color));

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
		for (size_t x = 0; x < width; ++x) {
			Color color = clamped_rgb(data[y * width + x]);
			sf::Color sf_color(color.x, color.y, color.z);
			image.setPixel(x, y, sf_color);
		}
	}

	texture.loadFromImage(image);
}
