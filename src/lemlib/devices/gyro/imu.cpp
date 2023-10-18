#include <cmath>
#include "lemlib/util.hpp"
#include "lemlib/devices/gyro/imu.hpp"

/**
 * Construct a new Imu
 *
 * Takes an unsigned integer for the port of the IMU. Pros imu is constructed
 * in an initializer list
 */
lemlib::Imu::Imu(uint8_t port)
    : imu(pros::Imu(port)) {}

/**
 * Construct a new Imu
 *
 * Takes a reference to a pros Imu to be used. Pros imu is then constructed in an
 * initializer list
 */
lemlib::Imu::Imu(pros::Imu& imu)
    : imu(imu) {}

/**
 * Calibrate the Imu
 *
 * Imu calibration should take around 2-3 seconds if blocking.
 * After imu calibration, returns whether the imu calibrated successfully or not,
 * unless the function is non-blocking, in which case success is returned whether or not
 * the imu has calibrated successfully, as we can't look into the future
 */
bool lemlib::Imu::calibrate(bool blocking) {
    if (!isConnected()) return true; // return true if imu is not connected
    imu.reset(blocking);
    if (!blocking) return false; // return false if function is non-blocking
    return !isCalibrated();
}

/**
 * return wether the IMU is calibrating or not.
 *
 * Just a wrapper for the pros::Imu::is_calibrating() function
 */
bool lemlib::Imu::isCalibrating() const { return imu.is_calibrating(); }

/**
 * return whether the IMU has been calibrated
 *
 * This function checks if the Imu is connected, is not calibrating,
 */
bool lemlib::Imu::isCalibrated() { return (isConnected() && !imu.is_calibrating() && !std::isinf(imu.get_heading())); }

/**
 * return whether the IMU is installed
 *
 * Just a wrapper for the pros::Imu::is_installed() function
 */
bool lemlib::Imu::isConnected() { return imu.is_installed(); }

/**
 * return the heading of the imu in radians and in standard position
 */
float lemlib::Imu::getHeading() const { return (M_PI - degToRad(imu.get_heading())); }

/**
 * Get the rotation of the imu in radians and in standard position
 */
float lemlib::Imu::getRotation() const { return (M_PI - degToRad(imu.get_rotation())); }

/**
 * Set the rotation of the imu in radians and in standard position
 */
void lemlib::Imu::setRotation(float orientation) const { imu.set_rotation(radToDeg(M_PI - radToDeg(orientation))); }

/**
 * Wrapper function for pros::Imu.get_port()
 */
std::uint8_t lemlib::Imu::getPort() { return imu.get_port(); }