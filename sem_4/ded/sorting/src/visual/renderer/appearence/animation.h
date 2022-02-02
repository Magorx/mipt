#pragma once


#include "visual/animation/animation.h"
#include "texture.h"


class AppearenceAnimation : public AbstractAnimation, public AppearenceTexture {
    const std::vector<RTexture*> *frames;
    sf::RenderTexture cur_frame;
    sf::Sprite cur_sprite;
    double frame_duration;

public:
    AppearenceAnimation(const std::vector<RTexture*> *frames, double frame_duration, bool looped = true, double time_coef = 1);
    AppearenceAnimation(const std::vector<RTexture*> &frames, double frame_duration, bool looped = true, double time_coef = 1);

    virtual void tick(const double dt = 0, const double absolute_time = 0) override;

    virtual void activate() override;
};
