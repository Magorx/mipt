#ifndef COLOR_MAP
#define COLOR_MAP

#include "color.h"

class ColorMap {
	size_t width;
	size_t height;
	Color *data;

	ColorMap           (const ColorMap& other) = delete;
	ColorMap& operator=(const ColorMap& other) = delete;

	ColorMap ():
	statement(nullptr),
	node_true(nullptr),
	node_false(nullptr)
	{}
}

#endif // COLOR_MAP