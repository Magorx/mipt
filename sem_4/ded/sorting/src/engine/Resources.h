#pragma once


#include "visual/renderer/gate_type_aliases.h"
#include "visual/renderer/appearence.h"
#include "visual/color/smart_color.h"
#include "visual/style/style.h"


class MogaEngine;
class RedactorEngine;


struct AnimationResourse {
    std::vector<RTexture*> frames;

    ~AnimationResourse() {
        for (auto frame : frames) {
            delete frame;
        }
    }
};

struct AVMouseReactionResources {
    RTexture *idle;
    RTexture *hovered;
    RTexture *pressed;
};

struct ToolResources {
    RTexture *idle;
    RTexture *chosen;
    RTexture *hovered;
    RTexture *pressed;
};

struct res_UtilityBarResources {
    RTexture *bar;
    RTexture *underbar;
    RTexture *l_corner;
    RTexture *r_corner;

    AVMouseReactionResources *close_button;
    AVMouseReactionResources *hide_button;
};

struct res_WindowResources {
    res_UtilityBarResources *util_bar;
    RTexture *frame;
};

struct ResourcesHolder {
    MogaEngine *engine;
    RedactorEngine *app_engine;

    std::vector<RTexture*> created_textures;
    std::vector<Appearence*>  created_apprs;
    std::vector<RShader*> created_shaders;
    std::vector<Style*> created_styles;

    inline Appearence *add_appr(Appearence *appr) { created_apprs.push_back(appr); return appr; }

    template <typename StyleT>
    inline StyleT *add_style(StyleT *style) { created_styles.push_back(style); return style; }

    struct {
        struct {
            AVMouseReactionResources close;
            AVMouseReactionResources hide;
            AVMouseReactionResources b3d;

            AVMouseReactionResources plus;
            AVMouseReactionResources save;
            AVMouseReactionResources rgb;

            struct {
                AVMouseReactionResources right;
            } arrow;

            AVMouseReactionResources basic;
        } button;

        struct {
            ToolResources brush;
            ToolResources eraser;
        } tool;

        struct {
            res_UtilityBarResources basic;
        } util_bar;

        struct {
            res_WindowResources basic;

            struct {
                RColor basic_frame;
            } color;
        } window;

        RTexture *alpha_blue;
        RTexture *frame_gray;

        RTexture *dot;
        RTexture *stick_vert;
        RTexture *stick_horz;

        RTexture *transparency_squares;
    } texture;

    struct {
        RTexture *spectrum;

        struct {
            struct {
                RColor frame;
                RColor content;
                RColor cursor;
                RColor selection;
            } basic;
        } text_field;

        struct {
            RColor basic;
        } slider;
    } color;

    struct {
        AnimationResourse fan;
        AnimationResourse lightning_idle;
        AnimationResourse lightning_hover;
    } animation;

    struct {
        RFont *arial;
        RFont *montserrat;
        RFont *lunchtime;
        RFont *raster;
        RFont *gothic_pixels;
        RFont *pxlxxl;
        RFont *adventurer;
        RFont *tight_pixel;
        RFont *orange_kid;

        RFont *basic;

        struct {
            RColor basic_header;
            RColor basic_menu;
        } color;

        struct {
            int basic;
            int basic_header;
            int basic_menu;
        } size;

        struct {
            SmartColor *basic_header;
            SmartColor *basic_menu;
        } smart_color;
    } font;

    struct {
        RShader *rgb_mapping;
        RShader *negative;

        struct {
            const char *rgb_mapping;
        } name;
    } shader;

    ~ResourcesHolder();

    void init(RedactorEngine *engine_);

    AppearenceAnimation *create_animation(const std::vector<RTexture*> &frames, double frame_duration, bool looped = false, Vec2d transform = {1, 1}, double time_coef = 1);
    RTexture *create_color(RGBA color);

    RShader *create_frag_shader(const char *filename);
};

extern ResourcesHolder App;
