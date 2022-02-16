#ifndef RGBA_H
#define RGBA_H

#include <cstdint>
#include <string>

#define RGBA_PRINT3(rgba) rgba.r, rgba.g, rgba.b
#define RGBA_PRINT(rgba) rgba.r, rgba.g, rgba.b, rgba.a

union RGBA {
	unsigned char rgba[4];
	struct {
		unsigned char r;
		unsigned char g;
		unsigned char b;
        unsigned char a;
	};
	int i;
	uint32_t ui32;

	inline bool is_transp() {
		return ui32 == 0;
	}

	RGBA();
	RGBA(const int x);
	RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_ = 255);
	RGBA(const int r_, const int g_, const int b_, const int a_ = 255);
    RGBA(const std::string &hex);

	RGBA operator!() const;

	RGBA operator+(const RGBA &other) const;

	RGBA operator/(const float &coef) const;
	RGBA operator*(const float &coef) const;
};

std::string component_to_string(const RGBA &rgba, int comp);
std::string to_string(const RGBA &rgba, bool add_alpha=false);

extern const unsigned char CLRMAX;


#endif // RGBA_H
