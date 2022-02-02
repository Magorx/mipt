#include "texture.h"


AppearenceTexture::AppearenceTexture(const RTexture *texture, const Vec2d &object_size, bool to_fill) {
    rmode.texture = texture;

    if (to_fill) {
        Vec2d texture_size = {(double) texture->getSize().x, (double) texture->getSize().y};
        transform = object_size / texture_size;
    }
}

RColor AppearenceTexture::get_px_color(Vec2d shape_position) const {
    if (!rmode.texture) return {0, 0, 0, 0};
    shape_position *= scale * transform;
    
    if (shape_position.y() < 0) {
        shape_position.content[1] = saved_image.getSize().y + shape_position.y();
    }
    if (shape_position.x() < 0) {
        shape_position.content[0] = saved_image.getSize().y + shape_position.x();
    }

    shape_position.content[0] = fmin(fmax(shape_position.content[0], 0), saved_image.getSize().x - 1);
    shape_position.content[1] = fmin(fmax(shape_position.content[1], 0), saved_image.getSize().y - 1);

    auto sf_color = saved_image.getPixel(shape_position.x(), shape_position.y());
    return {sf_color.r, sf_color.g, sf_color.b, sf_color.a};
}
