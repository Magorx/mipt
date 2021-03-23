#ifndef COLOR_MAP_PACK4
#define COLOR_MAP_PACK4

#include <SFML/Graphics.hpp>
#include "color_p4.h"

class ColorMap {
public:
	size_t 	 real_width;
	size_t   width;
	size_t   height;
	ColorP4 *data;
	char is_broken;

	ColorMap           (const ColorMap& other) = delete;
	ColorMap& operator=(const ColorMap& other) = delete;

	ColorMap();
	bool ctor(const size_t width_, const size_t height_, ColorP4 *data_ = nullptr);

	void flush_to_texture(sf::Texture &texture);
};

#endif // COLOR_MAP_PACK4