#include "animation.h"


AppearenceAnimation::AppearenceAnimation(const std::vector<RTexture*> *frames, double frame_duration, bool looped, double time_coef) :
AbstractAnimation(frames ? frame_duration * frames->size() : 0, looped, time_coef),
frames(frames),
cur_frame(),
cur_sprite(),
frame_duration(frame_duration)
{
    cur_frame.setRepeated(true);

    if (frames && frames->size()) {
        auto size = (*frames)[0]->getSize();
        cur_frame.create(size.x, size.y);

        cur_sprite.setTexture(cur_frame.getTexture());
        // cur_sprite.scale(1, -1);
        cur_sprite.setTextureRect(sf::IntRect(0, size.y, size.x, -size.y));
    }
}

AppearenceAnimation::AppearenceAnimation(const std::vector<RTexture*> &frames, double frame_duration, bool looped, double time_coef) :
AppearenceAnimation(&frames, frame_duration, looped, time_coef)
{}

void AppearenceAnimation::tick(const double dt, const double absolute_time) { 
    AbstractAnimation::tick(dt, absolute_time);

    if (is_running()) {
        size_t cur_idx  = (size_t) (elapsed() / frame_duration);
        size_t next_idx = (size_t) (elapsed() / frame_duration) + 1;
        next_idx = next_idx >= frames->size() ? cur_idx : next_idx;

        cur_frame.clear({0, 0, 0, 0});

        sf::RenderStates state;

        double frac = (elapsed() - frame_duration * ((int) (elapsed() / frame_duration))) / frame_duration;

        cur_sprite.setTexture(*(*frames)[cur_idx]);
        cur_sprite.setColor(sf::Color(255, 255, 255, (unsigned char) (255 * (1))));
        cur_frame.draw(cur_sprite);

        cur_sprite.setTexture(*(*frames)[next_idx]);
        cur_sprite.setColor(sf::Color(255, 255, 255, (unsigned char) (255 * frac)));
        cur_frame.draw(cur_sprite);

        set_texture(&cur_frame.getTexture());
    } else {
        if (!frames->size()) return;

        if (start_time) {
            set_texture((*frames)[frames->size() - 1]);
        } else {
            set_texture((*frames)[0]);
        }
    }
};

void AppearenceAnimation::activate() {
    start();
}