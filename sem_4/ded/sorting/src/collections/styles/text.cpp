#include "text.h"


namespace StdStyle {

namespace Text {

    TextStyle *basic() {
        return new TextStyle {
            App.font.size.basic,
            App.font.basic,
            App.font.color.basic_header,
            {0, 0, 0, 0}
        };
    }

    TextStyle *header() {
        return new TextStyle {
            App.font.size.basic_header,
            App.font.basic,
            App.font.color.basic_header,
            {0, 0, 0, 0}
        };
    }

    TextStyle *menu() {
        return new TextStyle {
            App.font.size.basic_menu,
            App.font.basic,
            App.font.color.basic_menu,
            {0, 0, 0, 0}
        };
    }

}

}
