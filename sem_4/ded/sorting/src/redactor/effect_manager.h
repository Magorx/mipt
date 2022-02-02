#pragma once


#include "collections/effects/layer_permanent.h"

#include <vector>

class EffectManager {
    std::vector<PluginEffect*> effects;

public:
    EffectManager() {}

    ~EffectManager() {
        for (auto effect : effects) {
            delete effect;
        }
    }
    
    void add(PluginEffect *effect) {
        if (!effect) return;

        effects.push_back(effect);
    }

    const std::vector<PluginEffect*> &get_effects() { return effects; }
};
