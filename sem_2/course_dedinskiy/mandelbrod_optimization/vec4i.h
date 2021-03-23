#ifndef GENERAL_Color
#define GENERAL_Color

#include <cmath>

const int Color_MAX_RANDOM_TRIES_CNT = 50;
const int Color_INF = 1e9;

typedef int8_t ColorByte;

//=============================================================================
//<KCTF> Random for vector ====================================================

int color_randint();

//=============================================================================
//<KCTF> Vec2_header ==========================================================

struct Color {
    ColorByte r;
    ColorByte g;
    ColorByte b;
    ColorByte a;

    Color();
    Color(ColorByte r_ = 0, ColorByte g_ = 0, ColorByte b_ = 0, ColorByte a_ = 0);

    bool is_zero() const;

    ColorByte &operator[](const ColorByte i)      ;
    ColorByte  operator[](const ColorByte i) const;

    inline static Color random() {
        return {Color_randint(), Color_randint(), Color_randint()};
    }

    inline static Color random(ColorByte mn, ColorByte mx) {
        return {Color_randint(mn, mx), Color_randint(mn, mx), Color_randint(mn, mx)};
    }
};

Color operator+  (const Color &first);
Color operator-  (const Color &first);

Color operator+  (const Color &first, const Color &second);
Color operator-  (const Color &first, const Color &second);
Color operator*  (const Color &first, const Color &second);
Color operator/  (const Color &first, const Color &second);

Color operator*  (const Color &first, const ColorByte k);
Color operator*  (const int k, const Color &first);
Color operator/  (const Color &first, const int k);

Color operator+= (Color &first, const Color &second);
Color operator*= (Color &first, const Color &second);
Color operator*= (Color &first, const int k);

bool operator==  (const Color &first, const Color &second);
bool operator<   (const Color &first, const Color &second);

#endif // GENERAL_Color