#pragma once


#include <vector>

#include "color/smart_color.h"
#include "renderer/appearence.h"
#include "style/style.h"


class VisualResourceManager {
    std::vector<RTexture*> texture;
    std::vector<SmartColor*> smart_color;
    std::vector<Appearence*> appearence;
    std::vector<Style*> style;

public:
    VisualResourceManager &operator=(const VisualResourceManager &other) = delete; 

    ~VisualResourceManager() {
        for (auto res : texture) {
            delete res;
        }

        for (auto res : smart_color) {
            delete res;
        }

        for (auto res : texture) {
            delete res;
        }

        for (auto res : style) {
            delete res;
        }
    }

    inline void add(RTexture* res) { texture.push_back(res); }
    inline void add(SmartColor* res) { smart_color.push_back(res); }
    inline void add(Appearence* res) { appearence.push_back(res); }
    inline void add(Style* res) { style.push_back(res); }
};
