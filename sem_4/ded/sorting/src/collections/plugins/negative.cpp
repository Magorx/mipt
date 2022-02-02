#include "redactor/plugin_std/plugin_std.hpp"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = PUPPY::EFFECT;

const char *PNAME    = "Negator";
const char *PVERSION = "2.0";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Negotiates with you colors";

// ============================================================================ Resources

PUPPY::Shader *r_shader_neg = nullptr;

// ============================================================================ General

#include "plugin_interface.h"
 
// ============================================================================ Logic

PUPPY::Status MyPluginInterface::init(const PUPPY::AppInterface *app_interface, const std::filesystem::path&) {
    srand(time(NULL));

    APPI = app_interface;

    if (APPI->feature_level & PUPPY::SHADER_SUPPORT) {
        r_shader_neg = APPI->factory.shader->compile(
           "uniform sampler2D texture;                                                  \
            void main() {                                                               \
                vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);                     \
                vec4 color = vec4(1.0 - pixel.r, 1.0 - pixel.g, 1.0 - pixel.b, pixel.w);\
                gl_FragColor = color;                                                   \
            }                                                                           \
           ", PUPPY::FRAGMENT);
    }

    APPI->log("[plugin](%s) inited", PINFO.name);
    return PUPPY::OK;
}

PUPPY::Status MyPluginInterface::deinit() {
    if (r_shader_neg) {
        delete r_shader_neg;
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
    if (!target) return;

    if (r_shader_neg) {
        target->apply_shader(r_shader_neg);
        return;
    } else {
        auto pixels = target->get_pixels();
        size_t w = target->get_size().x;
        size_t h = target->get_size().y;
        size_t sz = w * h;
        for (size_t i = 0; i < sz; ++i) {
            PUPPY::RGBA c = pixels[i];
            pixels[i] = {
                (unsigned char) (255 - c.r),
                (unsigned char) (255 - c.g),
                (unsigned char) (255 - c.b),
                c.a,
            };
        }

        target->render_pixels({0, 0}, {w, h}, pixels, PUPPY::COPY);
        delete pixels;
    }

    delete target;
}

void MyPluginInterface::show_settings() const {}


void MyPluginInterface::draw(const PUPPY::Vec2f &pos) const {
    float    size = APPI->get_size();
    PUPPY::RGBA color = APPI->get_color();

    PUPPY::RenderMode rmode = { PUPPY::COPY, nullptr };
    auto preview = APPI->get_preview();

    preview->render_circle(pos, size, color, rmode);

    delete preview;
}
