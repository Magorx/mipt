#ifndef COLOR_MAP
#define COLOR_MAP

#include <SFML/Graphics.hpp>
#include "color.h"

class ColorMap {
	size_t width;
	size_t height;
	Color *data;

	ColorMap           (const ColorMap& other) = delete;
	ColorMap& operator=(const ColorMap& other) = delete;

	ColorMap();

	bool ctor(const size_t width_, const size_t height_, Color *data_ = nullptr);

	void flush_to_texture(sf::Texture &texture);
};

#endif // COLOR_MAP