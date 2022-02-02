#include "window.h"
#include "util_bar.h"

namespace StdStyle {

namespace Window {

    WindowStyle *basic() {
        return new WindowStyle {
            StdStyle::UtilBar::basic(),
            
            new AppearenceTexture(App.texture.window.basic.frame)
        };
    }

    WindowStyle *dialog() {
        return basic();
    }

}

}
