#ifndef __INPUT_ENUMS_HPP__
#define __INPUT_ENUMS_HPP__

#include "../error/error_macros.hpp"

#include <cstdint>

enum class HatDir {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
    MAX = 4,
};

enum class HatMask {
    CENTER = 0,
    UP = 1,
    RIGHT = 2,
    DOWN = 4,
    LEFT = 8,
};

enum class JoyAxis {
    INVALID = -1,
    LEFT_X = 0,
    LEFT_Y = 1,
    RIGHT_X = 2,
    RIGHT_Y = 3,
    TRIGGER_LEFT = 4,
    TRIGGER_RIGHT = 5,
    SDL_MAX = 6,
    MAX = 10,
};

enum class JoyButton {
    INVALID = -1,
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    BACK = 4,
    GUIDE = 5,
    START = 6,
    LEFT_STICK = 7,
    RIGHT_STICK = 8,
    LEFT_SHOULDER = 9,
    RIGHT_SHOULDER = 10,
    DPAD_UP = 11,
    DPAD_DOWN = 12,
    DPAD_LEFT = 13,
    DPAD_RIGHT = 14,
    MISC1 = 15,
    PADDLE1 = 16,
    PADDLE2 = 17,
    PADDLE3 = 18,
    PADDLE4 = 19,
    TOUCHPAD = 20,
    SDL_MAX = 21,
    MAX = 128,
};

enum class MIDIMessage {
    NONE = 0,
    NOTE_OFF = 0x8,
    NOTE_ON = 0x9,
    AFTERTOUCH = 0xA,
    CONTROL_CHANGE = 0xB,
    PROGRAM_CHANGE = 0xC,
    CHANNEL_PRESSURE = 0xD,
    PITCH_BEND = 0xE,
    SYSTEM_EXCLUSIVE = 0xF0,
    QUARTER_FRAME = 0xF1,
    SONG_POSITION_POINTER = 0xF2,
    SONG_SELECT = 0xF3,
    TUNE_REQUEST = 0xF6,
    TIMING_CLOCK = 0xF8,
    START = 0xFA,
    CONTINUE = 0xFB,
    STOP = 0xFC,
    ACTIVE_SENSING = 0xFE,
    SYSTEM_RESET = 0xFF,
};

enum class MouseButton {
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    MIDDLE = 3,
    WHEEL_UP = 4,
    WHEEL_DOWN = 5,
    WHEEL_LEFT = 6,
    WHEEL_RIGHT = 7,
    MB_XBUTTON1 = 8,
    MB_XBUTTON2 = 9,
};

enum class MouseButtonMask {
    NONE = 0,
    LEFT = (1 << (int32_t(MouseButton::LEFT) - 1)),
    RIGHT = (1 << (int32_t(MouseButton::RIGHT) - 1)),
    MIDDLE = (1 << (int32_t(MouseButton::MIDDLE) - 1)),
    MB_XBUTTON1 = (1 << (int32_t(MouseButton::MB_XBUTTON1) - 1)),
    MB_XBUTTON2 = (1 << (int32_t(MouseButton::MB_XBUTTON2) - 1)),
};

inline MouseButtonMask mouse_button_to_mask(MouseButton button) {
    ERROR_FAIL_COND_V(button == MouseButton::NONE, MouseButtonMask::NONE);

    return MouseButtonMask(1 << ((int32_t)button - 1));
}

#endif
