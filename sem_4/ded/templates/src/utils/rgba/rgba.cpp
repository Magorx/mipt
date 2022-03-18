#include "rgba.h"

RGBA::RGBA() :
i(0)
{}

RGBA::RGBA(const int x) :
i(x)
{}

RGBA::RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_):
r(r_),
g(g_),
b(b_),
a(a_)
{}

RGBA::RGBA(const int r_, const int g_, const int b_, const int a_):
r((unsigned char) (r_ % 256)),
g((unsigned char) (g_ % 256)),
b((unsigned char) (b_ % 256)),
a((unsigned char) (a_ % 256))
{}

int hex_to_int(char c) {
    if ('0' <= c && c <= '9') {
        c -= '0';
    } else if ('a' <= c && c <= 'f') {
        c -= 'a' - 10;
    } else if ('A' <= c && c <= 'F') {
        c -= 'A' - 10;
    } else {
        c = 0;
    }
    return c;
}

int hex2_to_component(const char *hex2) {
    return hex_to_int(hex2[0]) * 16 + hex_to_int(hex2[1]);
}

RGBA::RGBA(const std::string &hex) :
r(0),
g(0),
b(0),
a(0)
{
    
    auto length = hex.length();
    if (!(length >= 6 && length <= 9)) {
        return;
    }

    r = hex2_to_component(hex.data() + 0);
    g = hex2_to_component(hex.data() + 2);
    b = hex2_to_component(hex.data() + 4);

    if (hex.length() == 8) {
        a = hex2_to_component(hex.data() + 6);
    }
}

RGBA RGBA::operator!() const {
    return {(unsigned char) (255 - r),
            (unsigned char) (255 - g),
            (unsigned char) (255 - b),
            (unsigned char) (a)};
}

RGBA RGBA::operator+(const RGBA &other) const {
    return {(unsigned char) (r + other.r),
            (unsigned char) (g + other.g),
            (unsigned char) (b + other.b),
            (unsigned char) (a + other.a)};
}

RGBA RGBA::operator/(const float &coef) const {
    return {(unsigned char) (r / coef),
            (unsigned char) (g / coef),
            (unsigned char) (b / coef),
            (unsigned char) (a / coef)};
}

RGBA RGBA::operator*(const float &coef) const {
    return {(unsigned char) (r * coef),
            (unsigned char) (g * coef),
            (unsigned char) (b * coef),
            (unsigned char) (a * coef)};
}

std::string component_to_string(const RGBA &rgba, int comp) {
    if (comp < 0 || comp > 3) return 0;

    int x = rgba.rgba[comp];
    
    int x1 = x / 16;
    int x2 = x % 16;

    std::string s = to_string(x1) + to_string(x2);

    if (x1 > 9) {
        s[0] = 'a' + (x1 - 10);
    }
    if (x2 > 9) {
        s[1] = 'a' + (x2 - 10);
    }

    return s;
}

std::string to_string(const RGBA &rgba, bool add_alpha) {
    std::string s(6, '0');
    if (add_alpha) s.resize(8, '0');

    for (int comp = 0; comp < 3 + add_alpha; ++comp) {
        int x = rgba.rgba[comp];
    
        int x1 = x / 16;
        int x2 = x % 16;

        s[2 * comp] = '0' + x1;
        s[2 * comp + 1] =  '0' + x2;

        if (x1 > 9) {
            s[2 * comp] = 'a' + (x1 - 10);
        }
        if (x2 > 9) {
            s[2 * comp + 1] = 'a' + (x2 - 10);
        }
    }

    return s;
}

const unsigned char CLRMAX = 255;
