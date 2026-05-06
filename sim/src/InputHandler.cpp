#include "sim/InputHandler.hpp"
#include "pros/misc.h"
#include <algorithm>

namespace sim {

InputHandler::InputHandler() = default;

bool InputHandler::poll(SimState& state) {
    SDL_Event event;

    // Snapshot previous buttons for new-press detection
    state.controller.prev_buttons.store(state.controller.buttons.load());

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                state.running.store(false);
                return false;

            case SDL_KEYDOWN:
                handleKeyDown(event.key.keysym.sym, state);
                break;

            case SDL_KEYUP:
                handleKeyUp(event.key.keysym.sym, state);
                break;
        }
    }

    updateAxes(state);
    return true;
}

static void setButton(SimState& state, controller_digital_e_t btn, bool pressed) {
    uint32_t mask = 1u << static_cast<int>(btn);
    if (pressed)
        state.controller.buttons.fetch_or(mask);
    else
        state.controller.buttons.fetch_and(~mask);
}

void InputHandler::handleKeyDown(SDL_Keycode key, SimState& state) {
    switch (key) {
        // Drive keys
        case SDLK_w:      key_w_     = true; break;
        case SDLK_s:      key_s_     = true; break;
        case SDLK_a:      key_a_     = true; break;
        case SDLK_d:      key_d_     = true; break;
        case SDLK_UP:     key_up_    = true; break;
        case SDLK_DOWN:   key_down_  = true; break;
        case SDLK_LEFT:   key_left_  = true; break;
        case SDLK_RIGHT:  key_right_ = true; break;

        // Competition mode
        case SDLK_F5:
            state.comp_mode.store(CompMode::AUTONOMOUS);
            break;
        case SDLK_F6:
            state.comp_mode.store(CompMode::OPCONTROL);
            break;
        case SDLK_F7:
            state.comp_mode.store(CompMode::DISABLED);
            break;

        // Controller buttons
        case SDLK_l: setButton(state, E_CONTROLLER_DIGITAL_L1, true); break;
        case SDLK_SEMICOLON: setButton(state, E_CONTROLLER_DIGITAL_L2, true); break;
        case SDLK_p: setButton(state, E_CONTROLLER_DIGITAL_R1, true); break;
        case SDLK_LEFTBRACKET: setButton(state, E_CONTROLLER_DIGITAL_R2, true); break;
        case SDLK_1: setButton(state, E_CONTROLLER_DIGITAL_X, true); break;
        case SDLK_2: setButton(state, E_CONTROLLER_DIGITAL_B, true); break;
        case SDLK_3: setButton(state, E_CONTROLLER_DIGITAL_Y, true); break;
        case SDLK_4: setButton(state, E_CONTROLLER_DIGITAL_A, true); break;

        case SDLK_ESCAPE:
            state.running.store(false);
            break;
    }
}

void InputHandler::handleKeyUp(SDL_Keycode key, SimState& state) {
    switch (key) {
        case SDLK_w:     key_w_     = false; break;
        case SDLK_s:     key_s_     = false; break;
        case SDLK_a:     key_a_     = false; break;
        case SDLK_d:     key_d_     = false; break;
        case SDLK_UP:    key_up_    = false; break;
        case SDLK_DOWN:  key_down_  = false; break;
        case SDLK_LEFT:  key_left_  = false; break;
        case SDLK_RIGHT: key_right_ = false; break;

        case SDLK_l: setButton(state, E_CONTROLLER_DIGITAL_L1, false); break;
        case SDLK_SEMICOLON: setButton(state, E_CONTROLLER_DIGITAL_L2, false); break;
        case SDLK_p: setButton(state, E_CONTROLLER_DIGITAL_R1, false); break;
        case SDLK_LEFTBRACKET: setButton(state, E_CONTROLLER_DIGITAL_R2, false); break;
        case SDLK_1: setButton(state, E_CONTROLLER_DIGITAL_X, false); break;
        case SDLK_2: setButton(state, E_CONTROLLER_DIGITAL_B, false); break;
        case SDLK_3: setButton(state, E_CONTROLLER_DIGITAL_Y, false); break;
        case SDLK_4: setButton(state, E_CONTROLLER_DIGITAL_A, false); break;
    }
}

void InputHandler::updateAxes(SimState& state) {
    // Left stick Y: W=forward(+127), S=backward(-127)
    int leftY = 0;
    if (key_w_) leftY += 127;
    if (key_s_) leftY -= 127;
    state.controller.leftY.store(leftY);

    // Left stick X: A=left(-127), D=right(+127)
    int leftX = 0;
    if (key_d_) leftX += 127;
    if (key_a_) leftX -= 127;
    state.controller.leftX.store(leftX);

    // Right stick Y: Up=forward, Down=backward
    int rightY = 0;
    if (key_up_)   rightY += 127;
    if (key_down_) rightY -= 127;
    state.controller.rightY.store(rightY);

    // Right stick X: Left/Right arrows
    int rightX = 0;
    if (key_right_) rightX += 127;
    if (key_left_)  rightX -= 127;
    state.controller.rightX.store(rightX);
}

} // namespace sim
