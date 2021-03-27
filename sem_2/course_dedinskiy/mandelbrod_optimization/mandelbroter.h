#ifndef MANDELBRODER
#define MANDELBRODER

#include "color_map_p4.h"
#include "intrinsic_types.h"

#define PACK_4 4
#define PACK_8 8

typedef float __attribute__((vector_size(sizeof(float) * PACK_4))) v_float_4;
typedef int   __attribute__((vector_size(sizeof(int  ) * PACK_4))) v_int_4;

const int MAX_ITTER = 256;

void render_mandelbrot(ColorMapP4 *cmap,
					   const float center_x, const float center_y,
					   const float width,    const float height,
					   const int max_itter = MAX_ITTER, const float max_radius = 10);

void render_mandelbrot_v_float_4(ColorMapP4 *cmap,
							  	 const float center_x, const float center_y,
							  	 const float width,    const float height,
							  	 const int max_itter = MAX_ITTER, const float max_radius = 10);

void render_mandelbrot_intrinsics(ColorMapP4 *cmap,
								  const float center_x, const float center_y,
								  const float width,    const float height,
								  const int max_itter = MAX_ITTER, const float max_radius = 10);

#endif // MANDELBRODER