#pragma once
#include <SDL2/SDL.h>
#include "SimState.hpp"

namespace sim {

class InputHandler {
public:
    InputHandler();
    // Returns false if SDL_QUIT was received
    bool poll(SimState& state);

private:
    bool key_w_{false}, key_s_{false};
    bool key_a_{false}, key_d_{false};
    bool key_up_{false}, key_down_{false};
    bool key_left_{false}, key_right_{false};

    void handleKeyDown(SDL_Keycode key, SimState& state);
    void handleKeyUp(SDL_Keycode key, SimState& state);
    void updateAxes(SimState& state);
};

} // namespace sim
