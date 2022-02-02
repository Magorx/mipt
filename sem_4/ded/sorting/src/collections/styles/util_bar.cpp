#include "util_bar.h"
#include "button.h"


namespace StdStyle {

namespace UtilBar {

    UtilityBarStyle *basic() {
        return new  UtilityBarStyle {
            StdStyle::Button::close(),
            StdStyle::Button::hide(),
            StdStyle::Button::basic_menu(),

            new AppearenceTexture(App.texture.util_bar.basic.bar),

            new AppearenceTexture(App.texture.util_bar.basic.l_corner),
            new AppearenceTexture(App.texture.util_bar.basic.r_corner, {-1, 1}),

            new AppearenceTexture(App.texture.util_bar.basic.underbar),
        };
    }

}

}
