#pragma once


#include "device/keyboard.h"


using Key = Keyboard::Key;


namespace HotkeyBind {

    const Hotkey none       =  Key::none;

    const Hotkey brush      =  Key::b;
    const Hotkey eraser     =  Key::e;
    const Hotkey pipette    =  Key::q;

    const Hotkey swap_color =  Key::x;
    const Hotkey swap_tools = {Key::x, Key::lctrl};

    const Hotkey alt_active_tool = Key::lalt;

    const Hotkey undo = {Key::z, Key::lctrl};
    const Hotkey redo = {Key::y, Key::lctrl};

}
