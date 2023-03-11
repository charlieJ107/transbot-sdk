//
// Created by Charlie on 08/03/2023.
//

#ifndef LIBTRANSBOT_PACKAGES_HPP
#define LIBTRANSBOT_PACKAGES_HPP

#include <cstdint>

namespace transbot_sdk
{


/**
 * @brief Adjusting the PID. For member P, I and D, they has been scale 1000 times as they are all decimal. The real
 * range is 0-10. This is a package to send.
 */
    typedef struct _pid_adjust
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x0A;
        const uint8_t function = 0x01;
        uint16_t P;
        uint16_t I;
        uint16_t D;
        uint8_t checksum;

        uint8_t get_save()
        {
            return save ? 0x5F : 0x00;
        }

        void set_save(bool save_value)
        {
            save = save_value;
        }

        void calculate_checksum()
        {
            checksum = (length + function + P + I + D + (save ? 0x5F : 0x00)) % 256;
        }

    private:
        bool save;
    } PID_Adjust;

/**
 * @brief Control bot moving. Linear and angular velocity are scale 100 times when transmit as they are decimal.
 * When angular velocity is 0, linear velocity > 0 means go straight, <0 means go back, = 0 means stop;
 * When linear velocity is 0, angular velocity > 0 turn left, < 0  turn right, =0 stop;
 * when linear and angular velocity are not 0, turn when moving;
 * This is a package to send;
 */
    typedef struct _move_control
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x06;
        const uint8_t function = 0x02;
        uint8_t linear_velocity;
        uint16_t angular_velocity;
        uint8_t checksum;

        void calculate_checksum()
        {
            checksum = (length + function + linear_velocity + angular_velocity) % 256;
        }
    } Move_Control;

    typedef struct _pwm_servo_control
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x03;
        enum Servo_ID : uint8_t
        {
            yaw = 0x01,
            pinch = 0x02
        } servo_id;
        uint8_t angle; //< When using depth camara, the angle is limited in [50-130], otherwise [0-180]
        uint8_t checksum;

        void calculate_checksum()
        {
            checksum = (length + function + servo_id + angle) % 256;
        }
    } PWM_Servo_Control;

    typedef struct _rgb_control
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x07;
        uint8_t function = 0x04;
        // 0-16 or 0xff (for all)
        uint8_t rgb_id;
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t checksum;

        void calculate_checksum()
        {
            checksum = (length + function + rgb_id + r + g + b) % 256;
        }
    } RGB_Control;

    typedef struct _rgb_effect
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x06;
        uint8_t function = 0x05;
        // Range: 0-6;
        uint8_t effect_type;
        // Range: 1-10; Set to 0xff to ignore.
        uint8_t frequency;
        // Range: 0-6; Set to 0xff to ignore.
        uint8_t param;
        uint8_t checksum;

        _rgb_effect(uint8_t effect_type_, uint8_t frequency_, uint8_t param_)
        {
            effect_type = effect_type_;
            frequency = frequency_;
            param = param_;
            checksum = (length + function + effect_type + frequency + param) % 256;
        }
    } RGB_Effect;

    typedef struct _buzzer
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x06;
        // 0: OFF
        // 1: ON
        // >=10 (10 * n) OFF after xx ms
        uint16_t time;
        uint8_t checksum;

        void calculate_checksum()
        {
            checksum = (length + function + time) % 256;
        }
    } Buzzer;

    typedef struct _led_light
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x04;
        uint8_t function = 0x07;
        // How light it is 0-100
        uint8_t light;
        uint8_t checksum;

        void calculate_checksum()
        {
            checksum = (length + function + light) % 256;
        }
    } LED_Light;

    typedef struct _auto_msg_setting
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x04;
        uint8_t function = 0x08;
        // 0x00: OFF
        // 0x01: ON
        uint8_t enable;
        // 0x00: save
        // 0x5F: Not save
        uint8_t save;
        uint8_t checksum;

        _auto_msg_setting(bool is_enable, bool pern_save)
        {
            enable = is_enable ? 0x01 : 0x00;
            save = pern_save ? 0x00 : 0x5F;
            checksum = (length + function + enable + save) % 256;
        }
    } Auto_Msg_Sending;

    typedef struct _moving_data_callback
    {
        uint16_t header = 0XFFFD;
        uint8_t length = 0x13;
        uint8_t function = 0x08;
        // -45 - +45
        int8_t linear_velocity;
        // -200 - +200
        int16_t angular_velocity;
        uint16_t acc_x;
        uint16_t acc_y;
        uint16_t acc_z;
        uint16_t gyro_x;
        uint16_t gyro_y;
        uint16_t gyro_z;
        // 10 times
        uint8_t battery_voltage;
        uint8_t checksum;
    } Moving_Msg_Callback;

// Test moto ok
    typedef struct _pwm_velocity
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x09;
        uint8_t moto_id; // 0x01 or 0x02;
        int16_t pwm; // -100 - +100
        uint8_t checksum;
    } PWM_Velocity;

    typedef struct _min_velocity_limit
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x0B;
        // 0-20
        uint8_t min_linear_velocity;
        // 0-100
        uint8_t min_angular_velocity;
        // ON: 0x00;
        // OFF: 0x5F;
        uint8_t save;
        uint8_t checksum;
    } Min_Velocity_Limit;

    typedef struct _gyro_direction
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x0D;
        // On: 0x01;
        // OFF: 0x00;
        uint8_t enable;
        uint8_t checksum;
    } Gyro_Direction;
}
#endif //LIBTRANSBOT_PACKAGES_HPP
