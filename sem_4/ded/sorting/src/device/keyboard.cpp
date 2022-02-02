#include "keyboard.h"

namespace Keyboard {
    const int KEYMAP_SIZE = 256;

    bool keymap_pressed[KEYMAP_SIZE];
}

const Hotkey Hotkey::none = {Keyboard::Key::none, 0};
