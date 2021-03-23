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
				cmap->data[yy * cmap->width + xx + i] = {255 - itter[i], 255 - itter[i], 255 - itter[i]};
			}
		}
	}
}

void render_mandelbrot_intrinsics(ColorMap *cmap,
								  const float center_x, const float center_y,
								  const float width, const float height,
								  const int max_itter, const float max_radius) {
	float mr2 = max_radius * max_radius;
	v_float4 maxrad2 = {mr2, mr2, mr2, mr2};
	float sx = center_x - width  / 2;
	float sy = center_y - height / 2;

	const v_float4 _255  = 255.f;

	float dx = width / cmap->width;
	float dy = height / cmap->height;

	v_float4 dx_mult = {dx, dx, dx, dx};
	v_float4 dx_add(3, 2, 1, 0);
	v_float4 dy_mult = {dy, dy, dy, dy};

	for (float y = 0; y < cmap->height; y += 1) {
		v_float4 y0 = {sy, sy, sy, sy};
		y0 = y0 + y * dy_mult;

		for (float x = 0; x < cmap->width; x += PACK_4) {
			v_float4 x0 = {sx, sx, sx, sx};
			x0 = x0 + (x + dx_add) * dx_mult;

			v_float4 X = x0;
			v_float4 Y = y0;

			v_int4 itter = {0, 0, 0, 0};
			int N = 0;
			for (;;) {
				++N;
				v_float4 x2 = X * X,
					     y2 = Y * Y,
					     xy = X * Y;

				v_float4 r2 = x2 + y2;

				v_int4 cmp = r2 <= maxrad2;
				if ((!+cmp) || N >= max_itter) break;
				itter = itter - cmp;

				X = x2 - y2 + x0;
				Y = xy + xy + y0;
			}

			size_t yy = (size_t) y;
			size_t xx = (size_t) x;

			v_float4 I = sqrt (sqrt (itter/max_itter)) * _255;

			for (int i = 0; i < PACK_4; ++i) {
				int c = (int) I[i];
				cmap->data[yy * cmap->width + xx + i] = {255 - c, c % 2 * 64, 255 - c};
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

			cmap->data[yy * cmap->width + xx] = {255 - itter, 255 - itter, 255 - itter};
		}
	}
}
