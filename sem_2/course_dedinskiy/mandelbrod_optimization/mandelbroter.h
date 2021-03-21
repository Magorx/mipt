#ifndef MANDELBRODER
#define MANDELBRODER

#include "color_map.h"

#define PACK_4 4
#define PACK_8 8

typedef float __attribute__((vector_size(sizeof(float) * PACK_4))) float4;
typedef int   __attribute__((vector_size(sizeof(int  ) * PACK_4))) int4;

typedef float __attribute__((vector_size(sizeof(float) * PACK_8))) float8;
typedef int   __attribute__((vector_size(sizeof(int  ) * PACK_8))) int8;

const int MAX_ITTER = 256;

void render_mandelbrot(ColorMap *cmap,
					   const float center_x, const float center_y,
					   const float width, const float height,
					   const int max_itter = MAX_ITTER, const float max_radius = 10);

void render_mandelbrot_float4(ColorMap *cmap,
							  const float center_x, const float center_y,
							  const float width, const float height,
							  const int max_itter = MAX_ITTER, const float max_radius = 10);

void render_mandelbrot_float8(ColorMap *cmap,
							  const float center_x, const float center_y,
							  const float width, const float height,
							  const int max_itter = MAX_ITTER, const float max_radius = 10);

#endif // MANDELBRODER