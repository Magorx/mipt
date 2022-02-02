#pragma once


#include <vector>
#include "effect.h"

#include "utils/logger.h"


template <typename T>
class Affected {
    friend Effect<T>;
    T *self;

    bool effects_owned = true;

protected:
    std::vector<Effect<T>*> effects;
    bool effects_applied = false;

public:
    Affected(T *self) : self(self) {}

    virtual ~Affected() {
        if (effects_owned) {
            for (auto effect : effects) {
                delete effect;
            }
        }
    }

    inline void set_effects_not_applied() { effects_applied = false; }

    virtual void add_effect(Effect<T>* effect) {
        if (!effects_owned) {
            logger.warning("Affected", "adding effect to affected that doesn't own its effects, clearing previous effects");
            effects_owned = true;
            effects.clear();
        }

        effects.push_back(effect);
    }

    inline std::vector<Effect<T>*> &get_effects() {
        return effects;
    }

    virtual void apply_effects() {
        if (effects_owned) {
            for (size_t i = 0; i < effects.size(); ++i) {
                effects[i]->apply();
            }
        } else {
            for (size_t i = 0; i < effects.size(); ++i) {
                effects[i]->apply_to(self, false);
            }
        }
    }

    virtual void set_effects(std::vector<Effect<T>*> &other_effects) {
        effects = other_effects;

        effects_applied = false;
        effects_owned = false;
    }

    virtual void apply(Effect<T> *effect, bool to_set_target = false) {
        effect->apply_to(self, to_set_target);
    }

    virtual void remove_effect(Effect<T>* effect) {
        for (size_t i = 0; i < effects.size(); ++i) {
            if (effects[i] == effect) {
                for (size_t j = i + 1; j < effects.size(); ++j) {
                    effects[j - 1] = effects[j];
                }
                effects.pop_back();
                break;
            }
        }
    }
};
