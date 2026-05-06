#include "main.h"

// ─────────────────────────────────────────────────────────────────────────────
// Robot hardware — edit ports to match your robot
// Port sign: negative = reversed direction
// ─────────────────────────────────────────────────────────────────────────────
pros::Motor left_motor(1);
pros::Motor right_motor(-2);   // port 2, reversed
pros::Controller master(E_CONTROLLER_MASTER);
pros::Imu imu(10);

/**
 * Runs once when the program starts.
 */
void initialize() {
    pros::lcd::initialize();
    pros::lcd::print(0, "VEX Sim initialized");
    pros::lcd::print(1, "F6=Drive  F5=Auto");
}

/**
 * Runs while the robot is disabled.
 */
void disabled() {}

/**
 * Runs before autonomous when connected to a competition switch.
 */
void competition_initialize() {}

/**
 * Autonomous routine — runs when F5 is pressed in the simulator.
 * This example drives forward 2 seconds, then turns right 1 second.
 */
void autonomous() {
    pros::lcd::print(0, "Autonomous running");

    // Drive forward
    left_motor.move_velocity(150);
    right_motor.move_velocity(150);
    pros::delay(2000);

    // Turn right
    left_motor.move_velocity(150);
    right_motor.move_velocity(-150);
    pros::delay(800);

    // Stop
    left_motor.move(0);
    right_motor.move(0);

    pros::lcd::print(0, "Autonomous done");
}

/**
 * Operator control — runs when F6 is pressed in the simulator.
 *
 * Tank drive: left stick Y controls left motors, right stick Y controls right motors.
 *
 * Keyboard mapping:
 *   W/S       = left stick Y  (left motors)
 *   Arrow Up/Down = right stick Y (right motors)
 *   A/D       = left stick X  (not used in tank drive)
 */
void opcontrol() {
    pros::lcd::print(0, "Driver control");

    while (true) {
        // Tank drive
        int left  = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
        int right = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);

        left_motor.move(left);
        right_motor.move(right);

        // Print heading to LCD every loop
        double heading = imu.get_heading();
        pros::lcd::print(1, "Heading: %.1f", heading);

        pros::delay(20);
    }
}
