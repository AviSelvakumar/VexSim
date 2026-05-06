#include "sim/SimState.hpp"

namespace sim {

SimState& SimState::get() {
    static SimState instance;
    return instance;
}

} // namespace sim
