#pragma once
#include <atomic>
#include <mutex>
#include <cstdint>
#include <cmath>

namespace sim {

constexpr int MAX_PORTS = 22;

enum class CompMode { DISABLED, AUTONOMOUS, OPCONTROL };

struct MotorState {
    std::atomic<int>    voltage{0};          // -127..127
    std::atomic<int>    millivolts{0};       // -12000..12000
    std::atomic<int>    target_velocity{0};  // RPM
    std::atomic<bool>   reversed{false};

    // Encoder position in degrees (stored as scaled int for lock-free atomics)
    // Actual degrees = position_millideg / 1000.0
    std::atomic<int64_t> position_millideg{0};

    enum class Mode      { VOLTAGE, MILLIVOLTS, VELOCITY } mode{Mode::VOLTAGE};
    enum class BrakeMode { COAST = 0, BRAKE = 1, HOLD = 2 };
    std::atomic<int> brake_mode_int{0}; // 0=COAST, 1=BRAKE/HOLD

    // Called by Physics each step. Returns the voltage this motor actually produces:
    //   - If commanded non-zero: pass through commanded voltage.
    //   - If commanded zero + BRAKE/HOLD: generate back-EMF opposing current motion.
    //   - If commanded zero + COAST: return 0 (motor disconnected).
    // speed_frac: current wheel speed as fraction of max (-1..1), positive = forward.
    double get_effective_voltage(double speed_frac) const {
        int cmd = voltage.load();
        if (cmd != 0) return cmd;
        if (brake_mode_int.load() >= 1)
            return -speed_frac * 127.0; // back-EMF opposes motion
        return 0.0; // COAST: motor disconnected
    }

    double get_position_deg() const {
        return position_millideg.load() / 1000.0;
    }
    void add_position_deg(double delta) {
        int64_t delta_int = static_cast<int64_t>(delta * 1000.0);
        position_millideg.fetch_add(delta_int);
    }
    void set_position_deg(double val) {
        position_millideg.store(static_cast<int64_t>(val * 1000.0));
    }
};

struct ControllerState {
    std::atomic<int> leftX{0};
    std::atomic<int> leftY{0};
    std::atomic<int> rightX{0};
    std::atomic<int> rightY{0};
    // Button bitmask — bit index matches controller_digital_e_t values
    std::atomic<uint32_t> buttons{0};
    // Track previous buttons for new-press detection
    std::atomic<uint32_t> prev_buttons{0};
};

struct RobotPose {
    double x{0.0};        // pixels
    double y{0.0};        // pixels
    double heading{0.0};  // radians, 0 = right (+X axis), CCW positive
    double heading_accumulated{0.0}; // unbounded, for rotation sensors
};

class SimState {
public:
    MotorState      motors[MAX_PORTS];  // index 1–21 match VEX port numbers
    ControllerState controller;

    mutable std::mutex pose_mutex;
    RobotPose pose;

    std::atomic<CompMode> comp_mode{CompMode::DISABLED};
    std::atomic<uint32_t> sim_millis{0};
    std::atomic<bool>     running{true};

    static SimState& get();

private:
    SimState() = default;
    SimState(const SimState&) = delete;
    SimState& operator=(const SimState&) = delete;
};

} // namespace sim
