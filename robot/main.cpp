#include "main.h"
#include "lemlib/api.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rotation.hpp"
#include "pros/vision.h"
#include "pros/vision.hpp"

pros::MotorGroup right_mg({1, 2, 3}, pros::MotorGearset::blue);
pros::MotorGroup left_mg({4, 5, 6}, pros::MotorGearset::blue);
pros::Motor upper_intake = pros::Motor(-7, pros::MotorGearset::blue);
pros::Motor lower_intake = pros::Motor(9, pros::MotorGearset::blue);


pros::adi::DigitalOut descore('B');
pros::adi::DigitalOut match_load('A');

pros::Vision vision_sensor(1, pros::E_VISION_ZERO_CENTER);


enum ScoreIntent {
	NONE,
	LOWER,
	STORE,
	UPPER,
	COUNT
};

lemlib::Drivetrain drivetrain(&left_mg, // left motor group
                              &right_mg, // right motor group
                              12.125, // 10 inch track width
                              lemlib::Omniwheel::OLD_325, // using new 4" omnis
                              400, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

pros::Imu imu_sensor(15);
pros::Rotation vertical_encoder(16);
pros::Rotation lateral_encoder(17);

lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder,
												  lemlib::Omniwheel::NEW_2, // wheel diameter in inches
												  0 // TBD
);
lemlib::TrackingWheel lateral_tracking_wheel(&lateral_encoder,
												  lemlib::Omniwheel::NEW_2, // wheel diameter in inches
												  0 // TBD
);


lemlib::OdomSensors sensors(&vertical_tracking_wheel, // no lateral encoder
							nullptr, // no back encoder
							&lateral_tracking_wheel,  // no inertial sensor
							nullptr,
							&imu_sensor
);

lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	chassis.setPose(0, 0, 0);
	chassis.moveToPoint(0, 1, 1000);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	ScoreIntent intent = NONE;
	bool descore_state = false;
	bool match_load_state = false;
	double vision_kP = 0.7;
	double vision_kI = 0.0;
	double vision_kD = 0.1;

	double vision_integral = 0;
	double vision_prev_error = 0;
	double vision_threshold = 10.0; // pixels

	while (true) {
		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		
		// Vision PID variables

		
		bool vision_enabled = master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

		if (vision_enabled) {
			pros::vision_object_s_t obj = vision_sensor.get_by_sig(0, 1);

			if (obj.signature != 0) {
				double error = obj.x_middle_coord; // 0 is centered

				vision_integral += error;
				double derivative = error - vision_prev_error;

				double output =
					vision_kP * error +
					vision_kI * vision_integral +
					vision_kD * derivative;

				// Deadzone
				if (std::abs(error) < vision_threshold) {
					output = 0;
					vision_integral = 0;
				}

				// Clamp turn value
				turn = std::clamp(-output, -127.0, 127.0);

				vision_prev_error = error;
				master.print(0, 0, "Vision X: %d", (int)error);
			} else {
				// No target found
				vision_integral = 0;
				vision_prev_error = 0;
				master.print(0, 0, "No target");
			}
		} else {
			// Reset PID when vision is off
			vision_integral = 0;
			vision_prev_error = 0;
		}
		chassis.arcade(turn, dir);                   // Calls arcade function with the values
		if (master.get_digital_new_press(DIGITAL_L1)) {
			intent = UPPER;
		} else if (master.get_digital_new_press(DIGITAL_L2)) {
			intent = STORE;
		} else if (master.get_digital_new_press(DIGITAL_R1)) {
			intent = LOWER;
		} else if (master.get_digital_new_press(DIGITAL_R2)) {
			intent = NONE;
		} else if (master.get_digital_new_press(DIGITAL_X)) {
			descore_state = !descore_state;
		} else if (master.get_digital_new_press(DIGITAL_B)) {
			match_load_state = !match_load_state;
		}

		descore.set_value(descore_state);
		match_load.set_value(match_load_state);
		switch (intent) {
			case NONE:
				upper_intake.move(0);
				lower_intake.move(0);
				break;
			case LOWER:
				upper_intake.move(-127);
				lower_intake.move(-127);
				break;
			case STORE:
				upper_intake.move(0);
				lower_intake.move(-127);
				break;
			case UPPER:
				upper_intake.move(127);
				lower_intake.move(127);
				break;
			default:
				break;
		}
		master.print(0, 0, "Intent: %d", intent);
		pros::delay(20);                               // Run for 20 ms then update

	}
}