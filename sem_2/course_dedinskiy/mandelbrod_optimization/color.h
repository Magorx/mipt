#ifndef COLOR
#define COLOR

#include "vec3d.h"
#include <cstdio>

extern const int 	i_MAXRGB;
extern const double d_MAXRGB;
extern const int 	i_MINRGB;
extern const double d_MINRGB;

using Color = Vec3d;
using Point = Vec3d;

Color randcolor(const int min = 0, const int max = 255);

Color rand_color_shift(const size_t module = 2);
Color rand_shifted_color(Color color, const size_t module = 2);
Color shift_color_randomly(Color &color, const size_t module = 2);

bool is_valid_color(Color color);

Color clamped_rgb(const Color color);

void print_rgb(Color color, const double gamma_correction = 0.5, FILE *file = stdout);
void save_rgb_to_ppm_image(FILE *fout, const Color *image, const size_t width, const size_t height, const double gamma_correction = 1);
void save_rgb_to_ppm_image(const char *filename, const Color *image, const size_t width, const size_t height, const double gamma_correction = 1);

#endif // COLOR