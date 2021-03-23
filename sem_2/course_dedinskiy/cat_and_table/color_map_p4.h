#ifndef COLOR_MAP_PACK4
#define COLOR_MAP_PACK4

#include <SFML/Graphics.hpp>
#include "color_p4.h"

class ColorMapP4 {
public:
	size_t 	 real_width;
	size_t   width;
	size_t   height;
	ColorP4 *data;
	char is_broken;

	ColorMapP4           (const ColorMapP4& other) = delete;
	ColorMapP4& operator=(const ColorMapP4& other) = delete;

	ColorMapP4();
	bool ctor(const size_t width_, const size_t height_, ColorP4 *data_ = nullptr);

	ARGB *operator[](const size_t i);

	void flush_to_texture(sf::Texture &texture);
};

#endif // COLOR_MAP_PACK4