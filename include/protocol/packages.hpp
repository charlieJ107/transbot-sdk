//
// Created by Charlie on 08/03/2023.
//

#ifndef TRANSBOT_PACKAGES_HPP
#define TRANSBOT_PACKAGES_HPP

#include <cstdint>

namespace transbot_sdk
{

    enum SavePermanently : uint8_t
    {
        True = 0x00,
        False = 0x5F
    };

    /**
     * @brief Adjusting the PID. For member P, I and D, they has been scale 1000 times as they are all decimal. The real
     * range is 0-10. This is a package to send.
     */
    typedef struct _pid_adjust
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x0A;
        const uint8_t function = 0x01;
        uint16_t P{};
        uint16_t I{};
        uint16_t D{};
        SavePermanently save{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + P + I + D + (save == True ? 0x5F : 0x00)) % 256;
        }

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
        uint8_t linear_velocity{};
        uint16_t angular_velocity{};
        uint8_t checksum{};

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
        } servo_id{};
        uint8_t angle{}; //< When using depth camara, the angle is limited in [50-130], otherwise [0-180]
        uint8_t checksum{};

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
        uint8_t rgb_id{};
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};
        uint8_t checksum{};

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
        uint8_t effect_type{};
        // Range: 1-10; Set to 0xff to ignore.
        uint8_t frequency{};
        // Range: 0-6; Set to 0xff to ignore.
        uint8_t param{};
        uint8_t checksum{};

        void calculate_checksum()
        {
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
        uint16_t time{};
        uint8_t checksum{};

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
        uint8_t light{};
        uint8_t checksum{};

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
        uint8_t enable{};
        // 0x00: save
        // 0x5F: Not save
        SavePermanently save{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + enable + save) % 256;
        }
    } Auto_Msg_Sending;

    typedef struct _moving_data_callback
    {
        uint16_t header = 0XFFFD;
        uint8_t length = 0x13;
        uint8_t function = 0x08;
        // -45 - +45
        int8_t linear_velocity{};
        // -200 - +200
        int16_t angular_velocity{};
        uint16_t acc_x{};
        uint16_t acc_y{};
        uint16_t acc_z{};
        uint16_t gyro_x{};
        uint16_t gyro_y{};
        uint16_t gyro_z{};
        // 10 times
        uint8_t battery_voltage{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum =
                    (length + function + linear_velocity + angular_velocity + acc_x + acc_y + acc_z + gyro_x + gyro_y +
                     gyro_z + battery_voltage) % 256;
        }
    } Moving_Msg_Callback;

    // For test moto ok
    typedef struct _pwm_velocity
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x09;
        uint8_t moto_id{}; // 0x01 or 0x02;
        int16_t pwm{}; // -100 - +100
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + moto_id + pwm) % 256;
        }
    } PWM_Velocity;

    typedef struct _min_velocity_limit
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x0B;
        // 0-20
        uint8_t min_linear_velocity{};
        // 0-100
        uint8_t min_angular_velocity{};
        // ON: 0x00;
        // OFF: 0x5F;
        uint8_t save{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + min_linear_velocity + min_angular_velocity + save) % 256;
        }
    } Min_Velocity_Limit;

    typedef struct _gyro_direction
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x0D;
        // On: 0x01;
        // OFF: 0x00;
        uint8_t enable{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + enable) % 256;
        }
    } Gyro_Direction;

    typedef struct _forward
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x04;
        uint8_t function = 0x0D;
        // -45- +45
        uint8_t velocity{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + velocity) % 256;
        }
    } Forward;

    typedef struct _servo_control
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x08;
        uint8_t function = 0x20;
        // 1-250 or 0xfe(all servo)
        uint8_t servo_id{};
        // 96-4000
        uint16_t target_position{};
        // 0-2000
        uint16_t time{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + servo_id + target_position + time) % 256;
        }
    } Servo_Control;

    typedef struct _set_servo_bus_id
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x09;
        uint8_t function = 0x21;
        // 0-250
        uint8_t servo_id{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + servo_id) % 256;
        }
    } Set_Servo_Bus_Id;

    /**
     * Enable servo torque(扭矩) on bus or not
     */
    typedef struct _enable_servo_torque
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x04;
        uint8_t function = 0x22;
        // 0: OFF
        // 1: ON
        uint8_t enable{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + enable) % 256;
        }
    } Enable_Servo_Torque;

    typedef struct _control_arm_joint_position
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x09;
        uint8_t function = 0x23;
        // Fist joint, ID = 7, 96-4000
        uint16_t joint1{};
        // Second joint, ID = 8, 96-4000
        uint16_t joint2{};
        // Third joint, ID = 9, 96-4000
        uint16_t joint3{};
        // Time for moving, less will be faster
        uint16_t time{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + joint1 + joint2 + joint3 + time) % 256;
        }
    } Control_Arm_Joint_Position;

    typedef struct _clear_flash_data
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x04;
        uint8_t function = 0xA0;
        uint8_t confirm = 0x5F;
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + confirm) % 256;
        }
    } Clear_Flash_Data;

    // Read data

    typedef struct _read_data_request
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x50;
        uint8_t data_type{};
        uint8_t param{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + data_type + param) % 256;
        }
    } Read_Data_Request;

    typedef struct _request_firmware_version
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x50;
        uint8_t data_type = 0x51;
        uint8_t param = 0x00;
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + data_type + param) % 256;
        }
    } Request_Firmware_Version;

    typedef struct _firmware_version_response
    {
        uint16_t header = 0xFFFD;
        uint8_t length = 0XFD;
        uint8_t data_type = 0x51;
        uint8_t major{};
        uint8_t minor{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + data_type + major + minor) % 256;
        }
    } Request_Firmware_Version_Response;

    // Request and response for Yaw, data_type = 0x52, param = 0x00,
    // Response with a uint16_t value
    typedef struct _request_yaw
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x50;
        uint8_t data_type = 0x52;
        uint8_t param = 0x00;
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + data_type + param) % 256;
        }
    } Request_Yaw;

    typedef struct _yaw_response
    {
        uint16_t header = 0xFFFD;
        uint8_t length = 0XFD;
        uint8_t data_type = 0x52;
        // Unit is radian and has been scaled to 1000 times, divide by 1000 to get the real value
        // then times 57.29 to get the degree
        uint16_t yaw{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + data_type + yaw) % 256;
        }
    } Yaw_Response;

    // Request and response for servo position, data_type = 0x53, param = servo id,
    // response with am uint8_t for servo id and a uint16_t for position.
    typedef struct _request_servo_position
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x06;
        uint8_t function = 0x20;
        uint8_t data_type = 0x53;
        // 1-250
        uint8_t servo_id{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + data_type + servo_id) % 256;
        }
    } Request_Servo_Position;

    typedef struct _servo_position_response
    {
        uint16_t header = 0xFFFD;
        uint8_t length = 0XFD;
        uint8_t data_type = 0x20;
        // 1-250
        uint8_t servo_id{};
        // 96-4000
        uint16_t position{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + data_type + servo_id + position) % 256;
        }
    } Servo_Position_Response;

    // Request and response for movement status, data_type = 0x08, param=0x00
    // response with uint8_t for linear velocity, uint16_t for angular velocity,
    // uint16 for x acceleration, uint16_t for y acceleration, uint16_t for z acceleration
    // uint16 for x gyro, uint16_t for y gyro, uint16_t for z gyro
    // uint8 for battery voltage
    typedef struct _request_movement_status
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x50;
        uint8_t data_type = 0x08;
        uint8_t param = 0x00;
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + data_type + param) % 256;
        }
    } Request_Movement_Status;

    typedef struct _movement_status_response
    {
        uint16_t header = 0xFFFD;
        uint8_t length = 0XFD;
        uint8_t data_type = 0x08;
        // -45-45
        uint8_t linear_velocity{};
        // -200-200
        uint8_t angular_velocity{};
        uint8_t x_acceleration{};
        uint8_t y_acceleration{};
        uint8_t z_acceleration{};
        uint8_t x_gyro{};
        uint8_t y_gyro{};
        uint8_t z_gyro{};
        // 10 times of the real value
        uint8_t battery_voltage{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + data_type + linear_velocity + angular_velocity + x_acceleration + y_acceleration +
                        z_acceleration + x_gyro + y_gyro + z_gyro + battery_voltage) % 256;
        }
    } Movement_Status_Response;

    // Request and response for PID parameters, data_type = 0x01, param = 0x00
    // response with uint16_t for P, uint16_t for I, uint16_t for D

    typedef struct _request_pid_parameters
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x50;
        uint8_t data_type = 0x01;
        uint8_t param = 0x00;
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + data_type + param) % 256;
        }
    } Request_PID_Parameters;

    typedef struct _pid_parameters_response
    {
        uint16_t header = 0xFFFD;
        uint8_t length = 0XFD;
        uint8_t data_type = 0x01;
        // 0-10000
        // 1000 times of the real value
        uint16_t P{};
        uint16_t I{};
        uint16_t D{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + data_type + P + I + D) % 256;
        }
    } PID_Parameters_Response;

    // Request and response for whether the current gyroscope assists with adjustment
    // data_type = 0x0C, param = 0x00
    // Response with uint8_t for 0 or 1

    typedef struct _request_gyro_assist
    {
        uint16_t header = 0xFFFE;
        uint8_t length = 0x05;
        uint8_t function = 0x50;
        uint8_t data_type = 0x0C;
        uint8_t param = 0x00;
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + function + data_type + param) % 256;
        }
    } Request_Gyro_Assist;

    typedef struct _gyro_assist_response
    {
        uint16_t header = 0xFFFD;
        uint8_t length = 0X04;
        uint8_t data_type = 0x0C;
        // 0 or 1
        uint8_t gyro_assist{};
        uint8_t checksum{};

        void calculate_checksum()
        {
            checksum = (length + data_type + gyro_assist) % 256;
        }
    } Gyro_Assist_Response;

}
#endif //TRANSBOT_PACKAGES_HPP
