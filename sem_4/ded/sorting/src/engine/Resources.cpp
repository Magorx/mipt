#include "Resources.h"

#include "engine/moga_engine.h"
#include "redactor/engine.h"

#include <cstring>


sf::Texture *load_texture(const char *filename) {
    sf::Texture *texture = new sf::Texture();
    if (!texture) {
        printf("can't alloc memory for texture [%s]\n", filename);
    }
    if (!texture->loadFromFile(filename)) {
        printf("can't load texture [%s]\n", filename);
    }

    texture->setRepeated(true);

    return texture;
}

sf::Texture *generate_color_texture(const RGBA color, int w = 32, int h = 32) {
    sf::Texture *texture = new sf::Texture();
    if (!texture) {
        printf("can't alloc memory for color (%d, %d, %d, %d)\n", color.r, color.g, color.b, color.a);
    }
    texture->create(w, h);

    RGBA *buffer = new RGBA[w * h];
    for (int i = 0; i < w * h; ++i) {
        buffer[i] = color;
    }

    texture->update((sf::Uint8*) buffer);

    delete[] buffer;

    return texture;
}

sf::Font *load_font(const char *filename) {
    sf::Font *font = new sf::Font;
    if (!font) {
        printf("can't alloc memory for font [%s]\n", filename);
    }
    if (!font->loadFromFile(filename)) {
        printf("sad story, can't load font [%s]\n", filename);
    }

    return font;
}

sf::Shader *load_frag_shader(const char *filename) {
    auto shader = new sf::Shader;
    if (!shader->loadFromFile(filename, sf::Shader::Fragment)) {
        printf("can't load shader\n");
    }
    shader->setUniform("texture", sf::Shader::CurrentTexture);

    return shader;
}

void load_animation(AnimationResourse &res, const std::vector<const char*> &frame_names) {
    for (size_t i = 0; i < frame_names.size(); ++i) {
        res.frames.push_back(new RTexture);
        res.frames[i]->loadFromFile(frame_names[i]);
    }
}

#define RES(path) "resources/" path
#define IMG(path) RES("image/" path)
#define BUTTON_IMG(name, path) IMG("button/" name "/" path)

#define FNT(path) RES("font/" path)

#define ANM(path) RES("animation/") path "/"

#define SHDR(path) RES("shader/") path


void ResourcesHolder::init(RedactorEngine *engine_) {
    shader.name.rgb_mapping = SHDR("rgb_mapping.glsl");

    engine = engine_;
    app_engine = engine_;

    texture.frame_gray = load_texture(IMG("frame_gray.png"));
    texture.dot = load_texture(IMG("dot.png"));
    texture.stick_vert = load_texture(IMG("stick_vert.png"));
    texture.stick_horz = load_texture(IMG("stick_horz.png"));

    texture.transparency_squares = load_texture(IMG("transparency_squares.png"));

    texture.button.basic.idle    = load_texture(BUTTON_IMG("basic", "idle.png"   ));
    texture.button.basic.hovered = load_texture(BUTTON_IMG("basic", "hovered.png"));
    texture.button.basic.pressed = load_texture(BUTTON_IMG("basic", "pressed.png"));

    texture.button.close.idle    = load_texture(BUTTON_IMG("close", "idle.png"   ));
    texture.button.close.hovered = load_texture(BUTTON_IMG("close", "hovered.png"));
    texture.button.close.pressed = load_texture(BUTTON_IMG("close", "pressed.png"));

    texture.button.hide.idle    = load_texture(BUTTON_IMG("hide", "idle.png"   ));
    texture.button.hide.hovered = load_texture(BUTTON_IMG("hide", "hovered.png"));
    texture.button.hide.pressed = load_texture(BUTTON_IMG("hide", "pressed.png"));

    texture.button.arrow.right.idle    = load_texture(BUTTON_IMG("arrow/right", "idle.png"   ));
    texture.button.arrow.right.hovered = load_texture(BUTTON_IMG("arrow/right", "hovered.png"));
    texture.button.arrow.right.pressed = load_texture(BUTTON_IMG("arrow/right", "pressed.png"));

    texture.button.plus.idle    = load_texture(BUTTON_IMG("plus", "idle.png"   ));
    texture.button.plus.hovered = load_texture(BUTTON_IMG("plus", "hovered.png"));
    texture.button.plus.pressed = load_texture(BUTTON_IMG("plus", "pressed.png"));

    texture.button.save.idle    = load_texture(BUTTON_IMG("save", "idle.png"   ));
    texture.button.save.hovered = load_texture(BUTTON_IMG("save", "hovered.png"));
    texture.button.save.pressed = load_texture(BUTTON_IMG("save", "pressed.png"));

    texture.button.rgb.idle    = load_texture(BUTTON_IMG("rgb", "idle.png"   ));
    texture.button.rgb.hovered = load_texture(BUTTON_IMG("rgb", "hovered.png"));
    texture.button.rgb.pressed = load_texture(BUTTON_IMG("rgb", "pressed.png"));

    font.arial         = load_font(FNT("Arial.ttf"        ));
    font.montserrat    = load_font(FNT("Montserrat.ttf"   ));
    font.lunchtime     = load_font(FNT("Lunchtime.ttf"    ));
    font.raster        = load_font(FNT("Raster.ttf"       ));
    font.gothic_pixels = load_font(FNT("GothicPixels.ttf" ));
    font.pxlxxl        = load_font(FNT("pxlxxl.ttf"       ));
    font.orange_kid    = load_font(FNT("OrangeKid.ttf"    ));
    font.adventurer    = load_font(FNT("Adventurer.ttf"   ));
    font.tight_pixel   = load_font(FNT("TightPixel.ttf"   ));

    font.basic = font.tight_pixel;

    font.color.basic_header = {255, 255, 255, 255};
    font.color.basic_menu   = {15, 15, 15, 255};

    const int font_size_delta = 7;
    font.size.basic        = 25 + font_size_delta;
    font.size.basic_header = 25 + font_size_delta;
    font.size.basic_menu   = 27 + font_size_delta;

    font.smart_color.basic_header = new SmartColor(font.color.basic_header);
    font.smart_color.basic_menu = new SmartColor(font.color.basic_menu);

    color.spectrum = load_texture("resources/image/color/spectrum.png");

    load_animation(animation.fan, {
        ANM("fan") "1.png",
        ANM("fan") "2.png",
        ANM("fan") "3.png",
        ANM("fan") "4.png",
        ANM("fan") "5.png",
        ANM("fan") "6.png",
        ANM("fan") "7.png",
        ANM("fan") "8.png",
    });

    load_animation(animation.lightning_idle, {
        ANM("lightning") "1.png",
        ANM("lightning") "2.png",
        ANM("lightning") "3.png",
        ANM("lightning") "4.png",
        ANM("lightning") "5.png",
        ANM("lightning") "6.png",
        ANM("lightning") "7.png",
        ANM("lightning") "8.png",
        ANM("lightning") "9.png",
        ANM("lightning") "10.png",
        ANM("lightning") "11.png",
        ANM("lightning") "12.png",
        ANM("lightning") "13.png",
        ANM("lightning") "14.png",
        ANM("lightning") "15.png",
        ANM("lightning") "16.png",

    });

    load_animation(animation.lightning_hover, {
        ANM("lightning_hover") "1.png",
        ANM("lightning_hover") "2.png",
        ANM("lightning_hover") "3.png",
        ANM("lightning_hover") "4.png",
        ANM("lightning_hover") "5.png",
        ANM("lightning_hover") "6.png",
        ANM("lightning_hover") "7.png",
        ANM("lightning_hover") "8.png",
        ANM("lightning_hover") "9.png",
        ANM("lightning_hover") "10.png",
        ANM("lightning_hover") "11.png",
        ANM("lightning_hover") "12.png",
        ANM("lightning_hover") "13.png",
        ANM("lightning_hover") "14.png",
        ANM("lightning_hover") "15.png",
        ANM("lightning_hover") "16.png",
    });

    texture.util_bar.basic.close_button = &texture.button.close;
    texture.util_bar.basic.hide_button  = &texture.button.hide;

    texture.util_bar.basic.bar      = load_texture(IMG("util_bar/basic/middle.png"));
    texture.util_bar.basic.underbar = load_texture(IMG("util_bar/basic/under.png"));
    texture.util_bar.basic.l_corner = load_texture(IMG("util_bar/basic/corner.png"));
    texture.util_bar.basic.r_corner = load_texture(IMG("util_bar/basic/corner.png"));

    color.text_field.basic.frame     = {177, 188, 255};
    color.text_field.basic.content   = {126, 158, 255};
    color.text_field.basic.cursor    = {20, 20, 20, 200};
    color.text_field.basic.selection = {120, 100, 145, 220};

    color.slider.basic = {180, 160, 190};

    texture.window.basic.util_bar = &texture.util_bar.basic;
    texture.window.color.basic_frame = {203, 219, 252, 220};
    texture.window.basic.frame = generate_color_texture(texture.window.color.basic_frame);

    shader.negative = load_frag_shader(SHDR("negative.glsl"));
    shader.rgb_mapping = load_frag_shader(SHDR("rgb_mapping.glsl"));
}

ResourcesHolder::~ResourcesHolder() {
    delete texture.alpha_blue;
    delete texture.frame_gray;

    delete texture.dot;
    delete texture.stick_vert;
    delete texture.stick_horz;

    delete texture.button.basic.idle   ;
    delete texture.button.basic.hovered;
    delete texture.button.basic.pressed;

    delete texture.button.close.idle   ;
    delete texture.button.close.hovered;
    delete texture.button.close.pressed;

    delete texture.button.hide.idle   ;
    delete texture.button.hide.hovered;
    delete texture.button.hide.pressed;

    delete texture.button.plus.idle   ;
    delete texture.button.plus.hovered;
    delete texture.button.plus.pressed;

    delete texture.button.save.idle   ;
    delete texture.button.save.hovered;
    delete texture.button.save.pressed;

    delete texture.button.rgb.idle   ;
    delete texture.button.rgb.hovered;
    delete texture.button.rgb.pressed;

    delete texture.button.arrow.right.idle   ;
    delete texture.button.arrow.right.hovered;
    delete texture.button.arrow.right.pressed;

    delete texture.window.basic.frame;

    delete font.arial;
    delete font.montserrat;
    delete font.lunchtime;
    delete font.raster;
    delete font.gothic_pixels;
    delete font.pxlxxl;
    delete font.orange_kid;
    delete font.adventurer;
    delete font.tight_pixel;

    delete color.spectrum;

    delete font.smart_color.basic_header;
    delete font.smart_color.basic_menu;

    for (auto appr : created_apprs) {
        delete appr;
    }

    for (auto txt : created_textures) {
        delete txt;
    }

    for (auto style : created_styles) {
        delete style;
    }

    delete shader.negative;
    delete shader.rgb_mapping;
}

AppearenceAnimation *ResourcesHolder::create_animation(const std::vector<RTexture*> &frames, double frame_duration, bool looped, Vec2d transform, double time_coef) {
    std::vector<RTexture*> *anim_frames = new std::vector<RTexture*>;
    anim_frames->reserve(frames.size());
    for (auto frame : frames) {
        anim_frames->push_back(frame);
    }

    AppearenceAnimation *animation = new AppearenceAnimation(anim_frames, frame_duration, looped, time_coef);
    animation->set_transform(transform);

    if (engine) {
        animation->tickable_nonfree = true;
        engine->add_tickable(animation);
    }

    return animation;
}

RTexture *ResourcesHolder::create_color(RGBA color) {
    auto color_texture = generate_color_texture(color);
    created_textures.push_back(color_texture);
    return color_texture;
}

RShader *ResourcesHolder::create_frag_shader(const char *filename) {
    auto shader = load_frag_shader(filename);
    created_shaders.push_back(shader);
    return shader;
}


ResourcesHolder App {nullptr, nullptr, {}, {}, {}, {}, {}, {}, {}, {}, {}};
