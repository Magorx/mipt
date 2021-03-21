#include "mandelbroter.h"

void render_mandelbrot_float4(ColorMap *cmap,
							  const float center_x, const float center_y,
							  const float width, const float height,
							  const int max_itter, const float max_radius) {
	float mr2 = max_radius * max_radius;
	float4 maxrad2 = {mr2, mr2, mr2, mr2};
	float sx = center_x - width  / 2;
	float sy = center_y - height / 2;

	float dx = width / cmap->width;
	float dy = height / cmap->height;

	float4 dx_mult = {dx, dx, dx, dx};
	float4 dx_add  = {0, 1, 2, 3};
	float4 dy_mult = {dy, dy, dy, dy};

	for (float y = 0; y < cmap->height; y += 1) {
		float4 y0 = {sy, sy, sy, sy};
		y0 += y * dy_mult;

		for (float x = 0; x < cmap->width; x += PACK_4) {
			float4 x0 = {sx, sx, sx, sx};
			x0 += (x + dx_add) * dx_mult;

			float4 X = x0;
			float4 Y = y0;

			int4 itter = {0, 0, 0, 0};
			int N = 0;
			for (;;) {
				++N;
				float4 x2 = X * X,
					   y2 = Y * Y,
					   xy = X * Y;

				float4 r2 = x2 + y2;

				int4 cmp = r2 < maxrad2;
				if (!(cmp[0] + cmp[1] + cmp[2] + cmp[3]) || N >= max_itter) break;
				itter -= cmp;

				X = x2 - y2 + x0;
				Y = xy + xy + y0;
			}

			size_t yy = (size_t) y;
			size_t xx = (size_t) x;

			for (int i = 0; i < PACK_4; ++i) {
				cmap->data[yy * cmap->width + xx + i] = {256.0 - itter[i], 256.0 - itter[i], 256.0 - itter[i]};
			}
		}
	}
}

void render_mandelbrot_float8(ColorMap *cmap,
							  const float center_x, const float center_y,
							  const float width, const float height,
							  const int max_itter, const float max_radius) {
	float mr2 = max_radius * max_radius;
	float8 maxrad2 = {mr2, mr2, mr2, mr2, mr2, mr2, mr2, mr2};
	float sx = center_x - width  / 2;
	float sy = center_y - height / 2;

	float dx = width / cmap->width;
	float dy = height / cmap->height;

	float8 dx_mult = {dx, dx, dx, dx, dx, dx, dx, dx};
	float8 dx_add  = {0, 1, 2, 3, 4, 5, 6, 7};
	float8 dy_mult = {dy, dy, dy, dy, dy, dy, dy, dy};

	for (float y = 0; y < cmap->height; y += 1) {
		float8 y0 = {sy, sy, sy, sy, sy, sy, sy, sy};
		y0 += y * dy_mult;

		for (float x = 0; x < cmap->width; x += PACK_4) {
			float8 x0 = {sx, sx, sx, sx};
			x0 += (x + dx_add) * dx_mult;

			float8 X = x0;
			float8 Y = y0;

			int8 itter = {0, 0, 0, 0, 0, 0, 0, 0};
			int N = 0;
			for (;;) {
				++N;
				float8 x2 = X * X,
					   y2 = Y * Y,
					   xy = X * Y;

				float8 r2 = x2 + y2;

				int8 cmp = r2 < maxrad2;
				if (!(cmp[0] + cmp[1] + cmp[2] + cmp[3] + cmp[4] + cmp[5] + cmp[6] + cmp[7]) || N >= max_itter) break;
				itter -= cmp;

				X = x2 - y2 + x0;
				Y = xy + xy + y0;
			}

			size_t yy = (size_t) y;
			size_t xx = (size_t) x;

			for (int i = 0; i < PACK_8; ++i) {
				cmap->data[yy * cmap->width + xx + i] = {256.0 - itter[i], 256.0 - itter[i], 256.0 - itter[i]};
			}
		}
	}
}

void render_mandelbrot(ColorMap *cmap,
					   const float center_x, const float center_y,
					   const float width, const float height,
					   const int max_itter, const float max_radius) {
	float mr2 = max_radius * max_radius;
	float sx = center_x - width  / 2;
	float sy = center_y - height / 2;

	float dx = width / cmap->width;
	float dy = height / cmap->height;

	for (float y = 0; y < cmap->height; y += 1) {
		float y0 = sy + y * dy;

		for (float x = 0; x < cmap->width; x += 1) {
			float x0 = sx + x * dx;

			float X = x0;
			float Y = y0;

			int itter = 0;
			int N = 0;
			for (;;) {
				++N;
				float x2 = X * X,
					  y2 = Y * Y,
					  xy = X * Y;

				float r2 = x2 + y2;

				if (r2 > mr2 || N >= max_itter) break;
				++itter;

				X = x2 - y2 + x0;
				Y = xy + xy + y0;
			}

			size_t yy = (size_t) y;
			size_t xx = (size_t) x;

			cmap->data[yy * cmap->width + xx] = {256.0 - itter, 256.0 - itter, 256.0 - itter};
		}
	}
}
