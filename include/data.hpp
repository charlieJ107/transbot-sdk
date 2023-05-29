#ifndef TRANSBOT_SDK_DATA_HPP
#define TRANSBOT_SDK_DATA_HPP

#include <cstdint>

namespace transbot_sdk
{
    typedef struct _motion_info
    {
        int linear_velocity{};
        int angular_velocity{};
        int x_acceleration{};
        int y_acceleration{};
        int z_acceleration{};
        int x_gyro{};
        int y_gyro{};
        int z_gyro{};
        int battery_voltage{};
    } Motion_Info;

    typedef struct _pid_parameters
    {
        double P{};
        double I{};
        double D{};
    } PID_Parameters;

    enum TRANSBOT_ENABLE : uint8_t
    {
        DISABLE = 0x00,
        ENABLE = 0x01
    };

    enum TRANSBOT_PERMANENT_SAVE : uint8_t
    {
        SAVE = 0x5F,
        NOT_SAVE = 0x00
    };

    enum TRANSBOT_CAMARA_CHANNEL : uint8_t
    {
        HORIZONTAL = 0x01,
        VERTICAL = 0x02,
    };

    enum TRANSBOT_ARM_SERVO_ID : uint8_t
    {
        JOINT1 = 0x07,
        JOINT2 = 0x08,
        JOINT3 = 0x09,
    };
}
#endif //TRANSBOT_SDK_DATA_HPP
