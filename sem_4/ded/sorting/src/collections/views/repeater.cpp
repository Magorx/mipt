#include "repeater.h"


v_Repeater::v_Repeater(const ViewBody &body, Appearence *appr, double stub_size) : // , int coord = 0) :
v_Highlighter(body),
stub_size(stub_size)
// coord(coord)
{
    set_appearence(appr);
}

void v_Repeater::render(Renderer *renderer) {
    int full_fits = body.size.x() / stub_size;

    for (int i = 0; i < full_fits; ++i) {
        double x = body.position.x() + stub_size * i;
        double y = body.position.y();

        renderer->set_appearence(appearence);
        renderer->apr_draw_rectangle({x, y}, {stub_size, body.size.y()});
    }

    double x = body.position.x() + stub_size * full_fits;
    double y = body.position.y();

    double x_size = body.size.x() - x;
    x_size += x_size < 0 ? stub_size : 0;
    
    renderer->set_appearence(appearence);
    renderer->apr_draw_rectangle({x, y}, {x_size, body.size.y()});
}