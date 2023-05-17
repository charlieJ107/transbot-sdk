#ifndef TRANSBOT_SDK_DATA_HPP
#define TRANSBOT_SDK_DATA_HPP
#include <cstdint>
namespace transbot_sdk
{
    typedef struct _motion_info
    {
        uint8_t linear_velocity{};
        uint16_t angular_velocity{};
        uint16_t x_acceleration{};
        uint16_t y_acceleration{};
        uint16_t z_acceleration{};
        uint16_t x_gyro{};
        uint16_t y_gyro{};
        uint16_t z_gyro{};
        uint8_t battery_voltage{};
    } Motion_Info;

    typedef struct _pid_parameters
    {
        uint16_t P{};
        uint16_t I{};
        uint16_t D{};
    } PID_Parameters;
}
#endif //TRANSBOT_SDK_DATA_HPP
