#include "target_factory.h"
#include "target.h"


PUPPY::RenderTarget *TargetFactory::spawn(const PUPPY::Vec2s &size, const PUPPY::RGBA &color) const {
    auto ret = new Target({(double) size.x, (double) size.y});
    ret->clear(color);

    return ret;
}

PUPPY::RenderTarget *TargetFactory::from_pixels(const PUPPY::Vec2s &size, const PUPPY::RGBA *data) const {
    auto ret = new Target({(double) size.x, (double) size.y});
    ret->update(data);

    return ret;
}

PUPPY::RenderTarget *TargetFactory::from_file(const char *path) const {
    auto ret = new Target(path);
    if (!ret || !ret->is_ok()) {
        delete ret;
        ret = nullptr;
    }

    return ret;
}
