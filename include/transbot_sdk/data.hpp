#ifndef TRANSBOT_SDK_DATA_HPP
#define TRANSBOT_SDK_DATA_HPP

#include <cstdint>

namespace transbot_sdk
{
    typedef struct _motion_info
    {
        double linear_velocity;
        double angular_velocity;
        double x_acceleration;
        double y_acceleration;
        double z_acceleration;
        double x_gyro;
        double y_gyro;
        double z_gyro;
        double battery_voltage;
        _motion_info(double linear_velocity, double angular_velocity, double x_acceleration, double y_acceleration,
                     double z_acceleration, double x_gyro, double y_gyro, double z_gyro, double battery_voltage) : linear_velocity(linear_velocity), angular_velocity(angular_velocity),
                                                                                                    x_acceleration(x_acceleration), y_acceleration(y_acceleration), z_acceleration(z_acceleration),
                                                                                                    x_gyro(x_gyro), y_gyro(y_gyro), z_gyro(z_gyro), battery_voltage(battery_voltage) {}
    } Motion_Info;

    typedef struct _pid_parameters
    {
        double P;
        double I;
        double D;
        _pid_parameters(double P, double I, double D) : P(P), I(I), D(D) {}
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
#endif // TRANSBOT_SDK_DATA_HPP
