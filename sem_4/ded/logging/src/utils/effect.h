#pragma once

template <typename T>
class affected;

template <typename T>
class Effect {
    friend T;

protected:
    T *target;

public:
    Effect(T *target) : target(target) {}
    virtual ~Effect() {}

    inline void set_target(T *new_target) { target = new_target; }

    virtual void apply() = 0;

    virtual void apply_to(T *affected, bool to_save_target = false) {
        if (to_save_target) {
            set_target(affected);
            apply();
        } else {
            T *main_target = target;

            set_target(affected);
            apply();

            set_target(main_target);
        }
    }
};
