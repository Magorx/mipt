#include "text.h"
#include "engine/moga_engine.h"


v_Text::v_Text(const ViewBody &body, const char *label_, int char_size, RGBA font_color, RGBA back_color, bool centrized, AbstractView *parent) :
AbstractView(body, parent),
label(nullptr),
char_size(char_size),
back_color(back_color),
font_color(font_color),
centrized(centrized)
{
    if (!label_) {
        printf("eror nullptr in the text colors\n");
    }

    label = strdup(label_);
}

v_Text::~v_Text() {
    free(label);
}

void v_Text::render(Renderer *renderer) {
    renderer->draw_text(label, char_size, body.position, font_color, back_color, centrized, centrized);
}
