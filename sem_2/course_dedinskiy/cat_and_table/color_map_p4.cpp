#include "color_map_p4.h"

ColorMapP4::ColorMapP4():
width(0),
height(0),
data(nullptr)
{}

bool ColorMapP4::ctor(const size_t width_, const size_t height_) {
	real_width = width_;
	width = real_width + real_width % 4;
	height = height_;
	data = (ARGB*) calloc(width * height, sizeof(ARGB));

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

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < real_width; ++x) {
			ARGB color = data[y * width + x];
			sf::Color sf_color(color.r, color.g, color.b);
			image.setPixel(x, y, sf_color);
		}
	}

	texture.loadFromImage(image);
}

ARGB *ColorMapP4::operator[](const size_t i) {
	return data + i * width;
}

const ARGB *ColorMapP4::operator[](const size_t i) const {
	return data + i * width;
}

void ColorMapP4::force_align_4(size_t &x) const {
	switch (x % 4) {
		case 0:
			return;
		case 1:
			--x;
			return;
		case 2:
			----x;
			return;
		case 3:
			++x;
			return;
		default:
			return;
	}
}

void ColorMapP4::crop_rectangle(size_t &x0, size_t &y0, size_t &x1, size_t &y1) const {
	x0 = std::max(x0, 0lu);
	y0 = std::max(y0, 0lu);
	x1 = std::min(x1, real_width);
	y1 = std::min(y1, height);

	x0 = std::min(x0, real_width);
	y0 = std::min(y0, height);
	x1 = std::max(x1, 0lu);
	y1 = std::max(y1, 0lu);

	if (x0 > x1) {
		std::swap(x0, x1);
	}
	if (y0 > y1) {
		std::swap(y0, y1);
	}
}

bool ColorMapP4::is_valid() const {
	return (((long int) data) % 16l == 0) && (width % 4 == 0);
}

void ColorMapP4::superimpose_alpha(const ColorMapP4 &cmap, size_t x0, size_t y0) {
	if (!is_valid() || !cmap.is_valid()) {
		printf("[ERR]<superimpose>: bad imagies provided\n");
		return;
	}
	
	force_align_4(x0);
	size_t x1 = x0 + cmap.width;
	size_t y1 = y0 + cmap.height;
	crop_rectangle(x0, y0, x1, y1);

	for (size_t y = y0, yy = 0; y < y1; ++y, ++yy) {
		for (size_t x = x0, xx = 0; x < x1; ++x, ++xx) {
			ARGB c1 = data[y * width + x];
			ARGB c2 = cmap[yy][xx];
			data[y * width + x] = mult((255 - c2.a) / 255.0f, c1) + mult(c2.a / 255.0f, c2);
		}
	}

}