#include "button.h"


inline MouseReactionStyle *basic_mouse_reaction_style(
    RTexture *idle,
    RTexture *hovered,
    RTexture *pressed,
    Vec2d transformation = {1, 1}
) {
    return new MouseReactionStyle {
        new AppearenceTexture(idle, transformation),
        new AppearenceTexture(pressed, transformation),
        App.create_animation({
            pressed,
            hovered,
        }, 0.1, false, transformation),
        App.create_animation({
            idle,
            hovered,
        }, 0.2, false, transformation),
        App.create_animation({
            hovered,
            idle,
        }, 0.2, false, transformation),
        nullptr
    };
}


namespace StdStyle {

namespace Button {

    MouseReactionStyle *close() {
        return basic_mouse_reaction_style(
            App.texture.button.close.idle,
            App.texture.button.close.hovered,
            App.texture.button.close.pressed
        );
    }

    MouseReactionStyle *hide() {
        return basic_mouse_reaction_style(
            App.texture.button.hide.idle,
            App.texture.button.hide.hovered,
            App.texture.button.hide.pressed
        );
    }

    MouseReactionStyle *basic_menu() {
        return basic_mouse_reaction_style(
            App.texture.button.basic.idle,
            App.texture.button.basic.hovered,
            App.texture.button.basic.pressed
        );
    }

    MouseReactionStyle *plus() {
        return basic_mouse_reaction_style(
            App.texture.button.plus.idle,
            App.texture.button.plus.hovered,
            App.texture.button.plus.pressed
        );
    }

    MouseReactionStyle *save() {
        return basic_mouse_reaction_style(
            App.texture.button.save.idle,
            App.texture.button.save.hovered,
            App.texture.button.save.pressed
        );
    }

    MouseReactionStyle *rgb() {
        return basic_mouse_reaction_style(
            App.texture.button.rgb.idle,
            App.texture.button.rgb.hovered,
            App.texture.button.rgb.pressed
        );
    }

    const RGBA SCND = {100, 120, 85};
    const RGBA MAIN = {200, 230, 130};

    MouseReactionStyle *red() {
        return basic_mouse_reaction_style(
            App.create_color({MAIN.r, SCND.r, SCND.r}),
            App.create_color({MAIN.g, SCND.g, SCND.g}),
            App.create_color({MAIN.b, SCND.b, SCND.b})
        );
    }

    MouseReactionStyle *green() {
        return basic_mouse_reaction_style(
            App.create_color({SCND.r, MAIN.r, SCND.r}),
            App.create_color({SCND.g, MAIN.g, SCND.g}),
            App.create_color({SCND.b, MAIN.b, SCND.b})
        );
    }

    MouseReactionStyle *blue() {
        return basic_mouse_reaction_style(
            App.create_color({SCND.r, SCND.r, MAIN.r}),
            App.create_color({SCND.g, SCND.g, MAIN.g}),
            App.create_color({SCND.b, SCND.b, MAIN.b})
        );
    }

    namespace Arrow {
        MouseReactionStyle *right() {
            return basic_mouse_reaction_style(
                App.texture.button.arrow.right.idle,
                App.texture.button.arrow.right.hovered,
                App.texture.button.arrow.right.pressed
            );
        }

        MouseReactionStyle *left() {
            return basic_mouse_reaction_style(
                App.texture.button.arrow.right.idle,
                App.texture.button.arrow.right.hovered,
                App.texture.button.arrow.right.pressed,
                {-1, 1}
            );
        }
    }

}

}
