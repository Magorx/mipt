#ifndef COLOR_MAP
#define COLOR_MAP

#include "color.h"

class ColorMap {
public:
	size_t width;
	size_t height;
	Color *data;

	ColorMap           (const ColorMap& other) = delete;
	ColorMap& operator=(const ColorMap& other) = delete;

	ColorMap ();
	bool ctor(int width_, int height_);

	void flush_to_texture(sf::Texture &texture);
}

#endif // COLOR_MAP