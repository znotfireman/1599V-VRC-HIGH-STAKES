#include "setup.h"

pros::Controller master (pros::E_CONTROLLER_MASTER);
//pros::Controller partner (pros::E_CONTROLLER_PARTNER);

// pros::Motor dt_motor_lf (DT_WHEEL_LF_PORT, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
// pros::Motor dt_motor_lm (DT_WHEEL_LM_PORT, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
// pros::Motor dt_motor_lb (DT_WHEEL_LB_PORT, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

// pros::Motor dt_motor_rf (DT_WHEEL_RF_PORT, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
// pros::Motor dt_motor_rm (DT_WHEEL_RM_PORT, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
// pros::Motor dt_motor_rb (DT_WHEEL_RB_PORT, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::MotorGroup dt_left (LEFT_MOTOR_PORTS, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::MotorGroup dt_right (RIGHT_MOTOR_PORTS, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::adi::Pneumatics clamp_solenoid (CLAMP_SOLENOID_PORT, true);
//pros::adi::DigitalOut indicator_g (INDICATOR_G_PORT);

pros::Motor intake_motor (INTAKE_PORT, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::IMU inertial_sensor (INERTIAL_SENSOR_PORT);

// drivetrain settings
lemlib::Drivetrain drivetrain {
    &dt_left, // left drivetrain motors
    &dt_right, // right drivetrain motors
    13.25, // track width
    lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
    450, // drivetrain rpm is 480
    4 // omni chase power is 2. If we had traction wheels, it would have been 8
};

// lateral motion controller

lemlib::ControllerSettings linearController (
    10, // proportional gain (kP)
    0, // integral gain (kI)
    55, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in inches
    100, // small error range timeout, in milliseconds
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController (
    2, // proportional gain (kP)
    0, // integral gain (kI)
    10, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in degrees
    100, // small error range timeout, in milliseconds
    3, // large error range, in degrees
    500, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew) prevent wheel cuz we dont have encoder
);

// sensors for odometry
// note that in this example we use internal motor encoders, so we don't pass vertical tracking wheels
lemlib::OdomSensors sensors {
    nullptr, // vertical tracking wheel 1, set to nullptr as we don't have one
    nullptr, // vertical tracking wheel 2, set to nullptr as we don't have one
    nullptr, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
    &inertial_sensor // inertial sensor
};

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors);