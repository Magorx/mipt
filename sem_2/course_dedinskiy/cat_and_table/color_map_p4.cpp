#include "color_map_p4.h"

ColorMapP4::ColorMapP4():
width(0),
height(0),
data(nullptr)
{}

bool ColorMapP4::ctor(const size_t width_, const size_t height_) {
	real_width = width_;
	width = real_width + real_width % 4;
	height = height_;
	data = (ARGB*) calloc(width * height, sizeof(ARGB));

	if (!data) {
		fprintf(stderr, "[ERR]<ColorMapP4>: data is not allocated in ctor\n");
		return false;
	} else {
		return true;
	}
}

bool ColorMapP4::ctor(const char *file) {
	sf::Image img;
	img.loadFromFile(file);

	auto size = img.getSize();
	size_t Width = size.x;
	size_t Height = size.y;
	ctor(Width, Height);

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < real_width; ++x) {
			sf::Color color = img.getPixel(x, y);
			data[y * width + x] = {color.r, color.g, color.b, color.a};
		}
	}

	return true;
}

void ColorMapP4::flush_to_texture(sf::Texture &texture) {
	sf::Image image;
	image.create(real_width, height);

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < real_width; ++x) {
			ARGB color = data[y * width + x];
			sf::Color sf_color(color.r, color.g, color.b);
			image.setPixel(x, y, sf_color);
		}
	}

	texture.loadFromImage(image);
}

ARGB *ColorMapP4::operator[](const size_t i) {
	return data + i * width;
}

const ARGB *ColorMapP4::operator[](const size_t i) const {
	return data + i * width;
}

void ColorMapP4::force_align_4(size_t &x) const {
	switch (x % 4) {
		case 0:
			return;
		case 1:
			--x;
			return;
		case 2:
			----x;
			return;
		case 3:
			++x;
			return;
		default:
			return;
	}
}

void ColorMapP4::crop_rectangle(size_t &x0, size_t &y0, size_t &x1, size_t &y1) const {
	x0 = std::max(x0, 0lu);
	y0 = std::max(y0, 0lu);
	x1 = std::min(x1, real_width);
	y1 = std::min(y1, height);

	x0 = std::min(x0, real_width);
	y0 = std::min(y0, height);
	x1 = std::max(x1, 0lu);
	y1 = std::max(y1, 0lu);

	if (x0 > x1) {
		std::swap(x0, x1);
	}
	if (y0 > y1) {
		std::swap(y0, y1);
	}
}

bool ColorMapP4::is_valid() const {
	return (((long int) data) % 16l == 0) && (width % 4 == 0);
}

void ColorMapP4::superimpose_alpha(const ColorMapP4 &cmap, size_t x0, size_t y0) {
	if (!is_valid() || !cmap.is_valid()) {
		printf("[ERR]<superimpose>: bad imagies provided\n");
		return;
	}
	
	force_align_4(x0);
	size_t x1 = x0 + cmap.width;
	size_t y1 = y0 + cmap.height;
	crop_rectangle(x0, y0, x1, y1);

	for (size_t y = y0, yy = 0; y < y1; ++y, ++yy) {
		for (size_t x = x0, xx = 0; x < x1; ++x, ++xx) {
			ARGB c1 = data[y * width + x];
			ARGB c2 = cmap[yy][xx];
			data[y * width + x] = mult((255 - c2.a) / 255.0f, c1) + mult(c2.a / 255.0f, c2);
		}
	}

}

void ColorMapP4::superimpose_alpha_intr(const ColorMapP4 &cmap, size_t x0, size_t y0) {
	if (!is_valid() || !cmap.is_valid()) {
		printf("[ERR]<superimpose>: bad imagies provided\n");
		return;
	}
	
	force_align_4(x0);
	size_t x1 = x0 + cmap.width;
	size_t y1 = y0 + cmap.height;
	crop_rectangle(x0, y0, x1, y1);

	const char ZZ = 0x80;

	__m256i pack_mask = _mm256_set_epi8(		ZZ, ZZ, ZZ, ZZ, ZZ, ZZ, ZZ, ZZ,
                                                31, 29, 27, 25, 23, 21, 19, 17,
                                                ZZ, ZZ, ZZ, ZZ, ZZ, ZZ, ZZ, ZZ,
                                                15, 13, 11, 9, 7, 5, 3, 1);

	__m256i alpha_mask = _mm256_set_epi8(		ZZ, 24, ZZ, 24, ZZ, 24, ZZ, 24,
                                                ZZ, 16, ZZ, 16, ZZ, 16, ZZ, 16,
                                                ZZ,  8, ZZ,  8, ZZ,  8, ZZ,  8,
                                                ZZ,  0, ZZ,  0, ZZ,  0, ZZ,  0);

	// __m256i zero = _mm256_setzero_si256();
    __m256i imax = _mm256_set1_epi16(255);

	for (size_t y = y0, yy = 0; y < y1; ++y, ++yy) {
		for (size_t x = x0, xx = 0; x < x1; x += 4, xx += 4) {
			__m128i fg_pxls = _mm_load_si128((__m128i*)(cmap.data + yy * cmap.width + xx));
			__m128i bg_pxls = _mm_load_si128((__m128i*)(data + y * width + x));

			__m256i fg = _mm256_cvtepu8_epi16(fg_pxls);
			__m256i bg = _mm256_cvtepu8_epi16(bg_pxls);

			__m256i alpha_fg = _mm256_shuffle_epi8(fg, alpha_mask);
            __m256i alpha_bg = _mm256_sub_epi16(imax, alpha_fg);

            // printf(") ");
            // for (int i = 0; i < 32; ++i) {
            // 	printf("%d ", ((unsigned char*)&fg)[i]);
            // }
            // printf("\n");

            // printf("* ");
            // for (int i = 0; i < 32; ++i) {
            // 	printf("%d ", ((unsigned char*)&alpha_fg)[i]);
            // }
            // printf("\n");


			fg = _mm256_mullo_epi16(fg, alpha_fg);

			// printf("> ");
   //          for (int i = 0; i < 32; ++i) {
   //          	printf("%d ", ((unsigned char*)&fg)[i]);
   //          }
   //          printf("\n");

			bg = _mm256_mullo_epi16(bg, alpha_bg);

			__m256i result_wide = _mm256_add_epi16(fg, bg);

			// printf("& ");
   //          for (int i = 0; i < 32; ++i) {
   //          	printf("%d ", ((unsigned char*)&result_wide)[i]);
   //          }
   //          printf("\n");
			result_wide = _mm256_shuffle_epi8(result_wide, pack_mask);
			// printf("] ");
   //          for (int i = 0; i < 32; ++i) {
   //          	printf("%d ", ((unsigned char*)&result_wide)[i]);
   //          }
   //          printf("\n");

			__m128i result_low  = _mm256_extractf128_si256(result_wide, 0);
            __m128i result_high = _mm256_extractf128_si256(result_wide, 1);

            __m128i result = (__m128i)_mm_movelh_ps((__m128)result_low, (__m128)result_high);

            unsigned char *result_ptr = (unsigned char*)&result;

            for (int i = 0; i < 4; ++i) {
            	unsigned char a = result_ptr[4 * i + 0];
                unsigned char r = result_ptr[4 * i + 1];
                unsigned char g = result_ptr[4 * i + 2];
                unsigned char b = result_ptr[4 * i + 3];
                data[y * width + x + i] = {r, g, b, a};
            }
		}
	}

}