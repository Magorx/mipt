#pragma once


#include "redactor/plugin_std/plugin_std.hpp"


class TargetFactory : public PUPPY::RenderTargetFactory {
    virtual PUPPY::RenderTarget *spawn       (const PUPPY::Vec2s &size, const PUPPY::RGBA &color = {0, 0, 0, 255}) const override;
    virtual PUPPY::RenderTarget *from_pixels (const PUPPY::Vec2s &size, const PUPPY::RGBA *data)            const override;
    virtual PUPPY::RenderTarget *from_file   (const char *path)                              const override;
};
