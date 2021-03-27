#ifndef COLOR_MAP_PACK4
#define COLOR_MAP_PACK4

#include <SFML/Graphics.hpp>
#include "color_p4.h"

class ColorMapP4 {
public:
	size_t 	 real_width;
	size_t   width;
	size_t   height;
	ARGB *data;

	ColorMapP4           (const ColorMapP4& other) = delete;
	ColorMapP4& operator=(const ColorMapP4& other) = delete;

	ColorMapP4();
	bool ctor(const size_t width_, const size_t height_);
	bool ctor(const char *file);

	bool detr();

	ARGB *operator[](const size_t i);
	const ARGB *operator[](const size_t i) const;

	size_t force_align_4(size_t x) const;
	void crop_rectangle(size_t &x0, size_t &y0, size_t &x1, size_t &y1) const;

	bool is_valid() const;

	void superimpose_alpha(const ColorMapP4 &cmap, size_t x0, size_t y0);
	void superimpose_alpha_intr(const ColorMapP4 &cmap, size_t x0, size_t y0);

	void flush_to_texture(sf::Texture &texture);
};

#endif // COLOR_MAP_PACK4