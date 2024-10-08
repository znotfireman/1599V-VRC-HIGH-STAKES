#include "setup.h"

int sgn (float number) { return 1 ? number >= 0 : -1 ; }

bool reverse_mode = false;

float driveCurve(float input, float scale) {
    if (scale != 0) {
        return int(powf(2.718, -(scale / 10) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) * input);
    }
    return std::round(input);
}

std::pair<float, float> arcade(int throttle, int turn, float curveGain = 0, float desaturateBias = 0.75) {
    throttle = driveCurve(throttle, curveGain);
    turn = driveCurve(turn, curveGain);

    // desaturate motors based on joyBias
    if (std::abs(throttle) + std::abs(turn) > 127) {
        int oldThrottle = throttle;
        int oldTurn = turn;
        throttle *= (1 - desaturateBias * std::abs(oldTurn / 127.0));
        turn *= (1 - (1 - desaturateBias) * std::abs(oldThrottle / 127.0));
        // ensure the sum of the two values is equal to 127
        // this check is necessary because of integer division
        if (std::abs(turn) + std::abs(throttle) == 126) {
            if (desaturateBias < 0.5) throttle += sgn(throttle);
            else turn += sgn(turn);
        }
    }
    
    float leftPower = driveCurve(throttle + turn, curveGain);
    float rightPower = driveCurve(throttle - turn, curveGain);
    return std::make_pair(leftPower, rightPower);
}

/*
std::pair<float, float> arcade(int throttle, int turn, float curveGain = 0) {
    turn *= 1;
    float leftPower = driveCurve(throttle + turn, curveGain);
    float rightPower = driveCurve(throttle - turn, curveGain);
    return std::make_pair(leftPower, rightPower);
}
*/

/*
std::pair<float, float> curvature(int throttle, int turn, float curveGain) {
    if (abs(throttle) < 4) {
        return arcade(throttle, turn, curveGain);;
    }

    float leftPower = throttle + (std::abs(throttle) * turn) / 127.0;
    float rightPower = throttle - (std::abs(throttle) * turn) / 127.0;

    leftPower = driveCurve(leftPower, curveGain);
    rightPower = driveCurve(rightPower, curveGain);

	return std::make_pair(leftPower, rightPower);
}
*/

void drive() {
	while (true) {
        double power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		double turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) { reverse_mode = !reverse_mode; }
        if (reverse_mode){power *= -1;}

		auto [left, right] = arcade(power, turn, 7.2);
        // auto [left, right] = curvature(power, turn, 7.2);

	    dt_left.move(left);
	    dt_right.move(right);

		pros::delay(20 ? CONTROLLER_MODE == bluetooth : 50);
	}
}

//bool fix = true;

void intake () {
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intake_motor.move_velocity(600);
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intake_motor.move_velocity(-400);
        } else {
            intake_motor.move_velocity(0);
        }
        //if (intake_motor.get_efficiency() < 10 && fix) {
            //intake_motor.move_relative(-360, 200);
            //pros::delay(500); //300ms per 360deg
        //}
        // if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
        //     fix = !fix;
        // }
        pros::delay(30 ? CONTROLLER_MODE == bluetooth : 50);
    }
}

void clamp() {
    while (true) {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
            clamp_solenoid.toggle();
        }
        pros::delay(30 ? CONTROLLER_MODE == bluetooth : 50);
    }
}


// PTO
        if (master.get_digital_new_press(DIGITAL_Y)) robot::setPTO(!isPtoActive);

// Arm
        if (master.get_digital_new_press(DIGITAL_B)) {
            arm.changeAngle(-9);
        } else if (partner.get_digital(DIGITAL_X)) arm.moveToAngle(50);
        else if (partner.get_digital(DIGITAL_A)) arm.moveToAngle(9);