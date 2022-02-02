#include "redactor/plugin_std/plugin_std.hpp"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = PUPPY::EFFECT;

const char *PNAME    = "Shurp";
const char *PVERSION = "2.0";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Edges pointy enough to make you bleed";

// ============================================================================ Resources

PUPPY::Shader *r_shader_blur;

// ============================================================================ General

#include "plugin_interface.h"
 
// ============================================================================ Logic

PUPPY::Status MyPluginInterface::init(const PUPPY::AppInterface *app_interface, const std::filesystem::path&) {
    srand(time(NULL));

    APPI = app_interface;

    if (!(APPI->feature_level & PUPPY::SHADER_SUPPORT)) {
        APPI->log("[plugin](%s) can't work without shaders and settings support, I'm sorry", PINFO.name);
        return PUPPY::ERR;
    }
    
    // if (APPI->general.feature_level & PFL_SETTINGS_SUPPORT) {

    //     APPI->settings.create_surface(SELF, 200, 200);
    //     r_setting_radius = APPI->settings.add(SELF, PST_TEXT_LINE, "Kernel");
    //     r_setting_power = APPI->settings.add(SELF, PST_TEXT_LINE, "Radius");
    //     r_setting_coef = APPI->settings.add(SELF, PST_TEXT_LINE, "Amount");
    // }

    r_shader_blur = APPI->factory.shader->compile(
"                                                                                  \
 uniform sampler2D texture;                                                        \
                                                                                   \
 uniform int ker_size;                                                             \
 uniform float kernel[10000];                                                      \
 uniform float tx_w;                                                               \
 uniform float tx_h;                                                               \
                                                                                   \
 vec3 rgb2hsl(vec3 color) {                                                        \
      vec3 hsl;                                                                    \
                                                                                   \
      float fmin = min(min(color.r, color.g), color.b);                            \
      float fmax = max(max(color.r, color.g), color.b);                            \
      float delta = fmax - fmin;                                                   \
                                                                                   \
      hsl.z = (fmax + fmin) / 2.0;                                                 \
                                                                                   \
      if (delta == 0.0)                                                            \
      {                                                                            \
          hsl.x = 0.0;                                                             \
          hsl.y = 0.0;                                                             \
      } else                                                                       \
      {                                                                            \
          if (hsl.z < 0.5)                                                         \
              hsl.y = delta / (fmax + fmin);                                       \
          else                                                                     \
              hsl.y = delta / (2.0 - fmax - fmin);                                 \
                                                                                   \
          float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;       \
          float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;       \
          float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;       \
                                                                                   \
          if (color.r == fmax)                                                     \
              hsl.x = deltaB - deltaG;                                             \
          else if (color.g == fmax)                                                \
              hsl.x = (1.0 / 3.0) + deltaR - deltaB;                               \
          else if (color.b == fmax)                                                \
              hsl.x = (2.0 / 3.0) + deltaG - deltaR;                               \
                                                                                   \
          if (hsl.x < 0.0)                                                         \
              hsl.x += 1.0;                                                        \
          else if (hsl.x > 1.0)                                                    \
              hsl.x -= 1.0;                                                        \
      }                                                                            \
                                                                                   \
      return hsl;                                                                  \
  }                                                                                \
                                                                                   \
                                                                                   \
                                                                                   \
 float hue2rgb(float f1, float f2, float hue) {                                    \
     if (hue < 0.0)                                                                \
         hue += 1.0;                                                               \
     else if (hue > 1.0)                                                           \
         hue -= 1.0;                                                               \
     float res;                                                                    \
     if ((6.0 * hue) < 1.0)                                                        \
         res = f1 + (f2 - f1) * 6.0 * hue;                                         \
     else if ((2.0 * hue) < 1.0)                                                   \
         res = f2;                                                                 \
     else if ((3.0 * hue) < 2.0)                                                   \
         res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;                         \
     else                                                                          \
         res = f1;                                                                 \
     return res;                                                                   \
 }                                                                                 \
                                                                                   \
 vec3 hsl2rgb(vec3 hsl) {                                                          \
     vec3 rgb;                                                                     \
                                                                                   \
     if (hsl.y == 0.0) {                                                           \
         rgb = vec3(hsl.z);                                                        \
     } else {                                                                      \
         float f2;                                                                 \
                                                                                   \
         if (hsl.z < 0.5)                                                          \
             f2 = hsl.z * (1.0 + hsl.y);                                           \
         else                                                                      \
             f2 = hsl.z + hsl.y - hsl.y * hsl.z;                                   \
                                                                                   \
         float f1 = 2.0 * hsl.z - f2;                                              \
                                                                                   \
         rgb.r = hue2rgb(f1, f2, hsl.x + (1.0/3.0));                               \
         rgb.g = hue2rgb(f1, f2, hsl.x);                                           \
         rgb.b = hue2rgb(f1, f2, hsl.x - (1.0/3.0));                               \
     }                                                                             \
     return rgb;                                                                   \
 }                                                                                 \
                                                                                   \
 vec3 rgb2hsv(vec3 c)                                                              \
 {                                                                                 \
     vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);                              \
     vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));             \
     vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));             \
                                                                                   \
     float d = q.x - min(q.w, q.y);                                                \
     float e = 1.0e-10;                                                            \
     return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);      \
 }                                                                                 \
                                                                                   \
 vec3 hsv2rgb(vec3 c)                                                              \
 {                                                                                 \
     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);                                \
     vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);                             \
     return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);                     \
 }                                                                                 \
                                                                                   \
 void main() {                                                                     \
     vec2 pos = gl_TexCoord[0].xy;                                                 \
     vec2 pixel_size = 1.0 / vec2(tx_w, tx_h);                                     \
                                                                                   \
     vec3 sum_color = vec3(0.0, 0.0, 0.0);                                         \
     float sum_weight = 0.0;                                                       \
                                                                                   \
     for (int dy = -ker_size, y = 0; dy <= ker_size; dy += 1, y += 1) {            \
         for (int dx = -ker_size, x = 0; dx <= ker_size; dx = dx + 1, x += 1) {    \
             vec2  cur_pos = pos + vec2(dx, dy) * pixel_size;                      \
             vec4  cur_col = texture2D(texture, cur_pos);                          \
             float cur_coef = kernel[(ker_size * 2 + 1) * y + x];                  \
                                                                                   \
             sum_color = sum_color + cur_col.xyz * cur_coef;                       \
             sum_weight = sum_weight + cur_coef;                                   \
         }                                                                         \
     }                                                                             \
                                                                                   \
     vec3 blur_color = sum_color / sum_weight;                                     \
     vec3 init_color = texture2D(texture, gl_TexCoord[0].xy).xyz;                  \
                                                                                   \
     vec3 color = init_color - (blur_color - init_color) * 0.1;                    \
                                                                                   \
     vec3 final = vec3(color);                                                     \
                                                                                   \
     gl_FragColor = vec4(final, texture2D(texture, gl_TexCoord[0].xy).w);          \
 }                                                                                 \
"
        , PUPPY::FRAGMENT);

    APPI->log("[plugin](%s) inited", PINFO.name);
    return PUPPY::OK;
}

PUPPY::Status MyPluginInterface::deinit() {
    if (r_shader_blur) {
        delete r_shader_blur;
    }
    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return PUPPY::OK;
}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const PUPPY::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double /*dt*/) const {
}

void MyPluginInterface::tool_on_press(const PUPPY::Vec2f &pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(const PUPPY::Vec2f &/*from*/, const PUPPY::Vec2f &to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(const PUPPY::Vec2f &/*pos*/) const {}

void MyPluginInterface::effect_apply() const {
    auto target = APPI->get_target();

    size_t w = 0, h = 0;
    w = target->get_size().x;
    h = target->get_size().y;
    
    int size = 2;
    float power = 1;
    float coef = 300;

    // PTextFieldSetting field;
    // if (r_setting_radius) {
    //     APPI->settings.get(SELF, r_setting_radius, &field);
    //     long long opt_size = read(field.text);
    //     if (opt_size > 0) {
    //         size = opt_size;
    //     }
    // }

    // if (r_setting_power) {
    //     APPI->settings.get(SELF, r_setting_power, &field);
    //     auto opt_power = read_float(field.text);
    //     if (opt_power > 0.01) {
    //         power = opt_power;
    //     }
    // }

    // if (r_setting_coef) {
    //     APPI->settings.get(SELF, r_setting_coef, &field);
    //     auto opt_coef = read_float(field.text);
    //     if (opt_coef > 0.01) {
    //         coef = opt_coef;
    //     }
    // }

    coef /= 100;
    power = 1 / power;

    int ker_size = size * 2 + 1;

    float *kernel = new float[ker_size * ker_size * 2];
    Vec2f center(size, size);
    
    int y = -1;
    for (int dy = -size; dy <= size; ++dy) {
        y++;
        int x = -1;
        for (int dx = -size; dx <= size; ++dx) {
            x++;
            float val = pow(1 - (center - Vec2f(x, y)).len() / (ker_size * coef), power);
            val = val > 0 ? val : 0;
            kernel[y * ker_size + x] = val;
        }
    }
    
    r_shader_blur->set_uniform_int("ker_size", (int) size);
    r_shader_blur->set_uniform_float_arr("kernel", kernel, ker_size * ker_size);
    r_shader_blur->set_uniform_float("tx_w", (float) w);
    r_shader_blur->set_uniform_float("tx_h", (float) h);

    target->apply_shader(r_shader_blur);

    delete[] kernel;
    delete target;
}

void MyPluginInterface::show_settings() const {}

void MyPluginInterface::draw(const PUPPY::Vec2f &pos) const {
    float    size = APPI->get_size();
    PUPPY::RGBA color = APPI->get_color();

    auto preview = APPI->get_preview();

    preview->render_circle(pos, size, color, PUPPY::COPY);

    delete preview;
}
