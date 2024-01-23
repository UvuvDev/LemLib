#include "lemlib/devices/motor/prosmotor.hpp"
#include "lemlib/devices/motor/abstractmotor.hpp"
#include "pros/abstract_motor.hpp"
#include <memory>

namespace lemlib {

PROSMotor::PROSMotor(const uint8_t port, const bool isReversed, float gearRatioArg, const int gearset,
                     std::shared_ptr<PROSMotor> pairMotor, std::shared_ptr<FAPID> pid) {
    this->motor = std::move(std::make_unique<pros::Motor>(port));
    switch (gearset) {
        case 600: this->motor->set_gearing(pros::E_MOTOR_GEARSET_06); break;
        case 200: this->motor->set_gearing(pros::E_MOTOR_GEARSET_18); break;
        case 100: this->motor->set_gearing(pros::E_MOTOR_GEARSET_36); break;
        default: this->motor->set_gearing(pros::E_MOTOR_GEARSET_06); break;
    }
    this->motor->set_reversed(isReversed);
    this->isReversed = isReversed;
    this->gearRatio = gearRatioArg;
    this->pairMotor = pairMotor;
    this->pid = pid;
    this->isBroken = false;
}

bool PROSMotor::getIsBroken() { return isBroken; }

float PROSMotor::getRPM() { return motor->get_actual_velocity() * gearRatio; }

float PROSMotor::getVoltage() { return voltage; }

int PROSMotor::getPosition() { return motor->get_position(); }

void PROSMotor::spinAtVoltage(int voltage) {
    if (voltage > 12000) voltage = 12000;
    if (voltage < -12000) voltage = -12000;

    if (!isBroken) { motor->move_voltage(voltage); }
}

void PROSMotor::spinPerc(int percent) {
    if (percent > 100) percent = 100;
    if (percent < -100) percent = -100;

    if (!isBroken) { motor->move(percent / 100 * 127); }
    std::cout << " Port Value: " << motor->get_port() << std::endl;
    // motor->move(127);
}

void PROSMotor::spinJoystick(int joystickValue) {
    if (!isBroken) { motor->move_voltage(joystickValue / 127 * 12000); }
}

void PROSMotor::spinAtRPM(int RPM) {
    if (!isBroken) { pid->update(getRPM(), RPM); }
}

void PROSMotor::spinUntilDegree(int degree, int speedInVoltage) {
    if (getPosition() < degree) {
        spinAtVoltage(speedInVoltage);
    } else {
        spinAtVoltage(0);
    }
}

void PROSMotor::spinFor(float seconds, int speedInVoltage) {
    if (!isBroken) {
        static float startTime = pros::millis();

        if (pros::millis() - startTime < seconds * 1000) {
            spinAtVoltage(speedInVoltage);
        } else {
            spinAtVoltage(0);
        }
    }
}

void PROSMotor::logMotorPerformance() {}

void PROSMotor::set_zero_position(int position) { motor->set_zero_position(position); }

void PROSMotor::shutDown() { isBroken = true; }

void PROSMotor::revive() { isBroken = false; }

bool PROSMotor::isOverheated() {
    if (motor->get_temperature() > overheatTempCelsius) {
        return true;
    } else {
        return false;
    }
}

int PROSMotor::getPort() { return motor->get_port(); }

void PROSMotor::setGearset(int gearset) { baseRPM = gearset; }

int PROSMotor::getGearset() { return baseRPM; }

void PROSMotor::setReversed(bool isReversed) { motor->set_reversed(isReversed); }

bool PROSMotor::getIsReversed() { return isReversed; }

std::shared_ptr<Abstract_Motor> PROSMotor::getPairMotor() { return pairMotor; }

void PROSMotor::setPID(std::shared_ptr<FAPID> pid) { this->pid = pid; }

std::shared_ptr<FAPID> PROSMotor::getPID() { return pid; }

void PROSMotor::setGearRatio(float gearRatio) { this->gearRatio = gearRatio; }

float PROSMotor::getGearRatio() { return gearRatio; }

} // namespace lemlib