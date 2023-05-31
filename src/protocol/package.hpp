#ifndef TRANSBOT_PACKAGES_HPP
#define TRANSBOT_PACKAGES_HPP

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

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
        uint16_t P{};
        uint16_t I{};
        uint16_t D{};
        uint8_t save{};
        uint8_t checksum{};
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
    } Move_Control;

    typedef struct _pwm_servo_control
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x03;
        uint8_t servo_id{};
        uint8_t angle{}; //< When using depth camara, the angle is limited in [50-130], otherwise [0-180]
        uint8_t checksum{};
    } PWM_Servo_Control;

    typedef struct _rgb_control
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x07;
        const uint8_t function = 0x04;
        // 0-16 or 0xff (for all)
        uint8_t rgb_id{};
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};
        uint8_t checksum{};
    } RGB_Control;

    typedef struct _rgb_effect
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x06;
        const uint8_t function = 0x05;
        // Range: 0-6;
        uint8_t effect_type{};
        // Range: 1-10; Set to 0xff to ignore.
        uint8_t frequency{};
        // Range: 0-6; Set to 0xff to ignore.
        uint8_t param{};
        uint8_t checksum{};
    } RGB_Effect;

    typedef struct _buzzer
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x06;
        // 0: OFF
        // 1: ON
        // >=10 (10 * n) OFF after xx ms
        uint16_t time{};
        uint8_t checksum{};
    } Buzzer;

    typedef struct _led_light
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x04;
        const uint8_t function = 0x07;
        // How light it is 0-100
        uint8_t light{};
        uint8_t checksum{};
    } LED_Light;

    typedef struct _auto_msg_setting
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x04;
        const uint8_t function = 0x08;
        // 0x00: OFF
        // 0x01: ON
        uint8_t enable{};
        // 0x00: save
        // 0x5F: Not save
        uint8_t save{};
        uint8_t checksum{};
    } Auto_Msg_Sending;

    typedef struct _moving_data_callback
    {
        const uint16_t header = 0XFFFD;
        const uint8_t length = 0x13;
        const uint8_t function = 0x08;
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
    } Moving_Msg_Callback;

    // For test moto ok
    typedef struct _pwm_velocity
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x09;
        uint8_t moto_id{}; // 0x01 or 0x02;
        int16_t pwm{}; // -100 - +100
        uint8_t checksum{};
    } PWM_Velocity;

    typedef struct _min_velocity_limit
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x0B;
        // 0-20
        uint8_t min_linear_velocity{};
        // 0-100
        uint8_t min_angular_velocity{};
        // ON: 0x00;
        // OFF: 0x5F;
        uint8_t save{};
        uint8_t checksum{};
    } Min_Velocity_Limit;

    typedef struct _gyro_direction
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x0D;
        // On: 0x01;
        // OFF: 0x00;
        uint8_t enable{};
        uint8_t checksum{};
    } Gyro_Direction;

    typedef struct _forward
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x04;
        const uint8_t function = 0x0D;
        // -45- +45
        int8_t velocity{};
        uint8_t checksum{};
    } Forward;

    typedef struct _servo_control
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x08;
        const uint8_t function = 0x20;
        // 1-250 or 0xfe(all servo)
        uint8_t servo_id{};
        // 96-4000
        uint16_t target_position{};
        // 0-2000
        uint16_t time{};
        uint8_t checksum{};
    } Servo_Control;

    typedef struct _set_servo_bus_id
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x09;
        const uint8_t function = 0x21;
        // 0-250
        uint8_t servo_id{};
        uint8_t checksum{};
    } Set_Servo_Bus_Id;

    /**
     * Enable servo torque(扭矩) on bus or not
     */
    typedef struct _enable_servo_torque
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x04;
        const uint8_t function = 0x22;
        // 0: OFF
        // 1: ON
        uint8_t enable{};
        uint8_t checksum{};
    } Enable_Servo_Torque;

    typedef struct _control_arm_joint_position
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x09;
        const uint8_t function = 0x23;
        // Fist joint, ID = 7, 96-4000
        uint16_t joint1{};
        // Second joint, ID = 8, 96-4000
        uint16_t joint2{};
        // Third joint, ID = 9, 96-4000
        uint16_t joint3{};
        // Time for moving, less will be faster
        uint16_t time{};
        uint8_t checksum{};
    } Control_Arm_Joint_Position;

    typedef struct _clear_flash_data
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x04;
        const uint8_t function = 0xA0;
        const uint8_t confirm = 0x5F;
        uint8_t checksum{};
    } Clear_Flash_Data;

    // Read data

    typedef struct _read_data_request
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x50;
        uint8_t data_type{};
        uint8_t param{};
        uint8_t checksum{};
    } Read_Data_Request;

    typedef struct _request_firmware_version
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x50;
        const uint8_t data_type = 0x51;
        const uint8_t param = 0x00;
        uint8_t checksum{};
    } Request_Firmware_Version;

    typedef struct _firmware_version_response
    {
        const uint16_t header = 0xFFFD;
        const uint8_t length = 0XFD;
        const uint8_t data_type = 0x51;
        uint8_t major{};
        uint8_t minor{};
        uint8_t checksum{};
    } Request_Firmware_Version_Response;

    // Request and response for Yaw, data_type = 0x52, param = 0x00,
    // Response with a uint16_t value
    typedef struct _request_yaw
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x50;
        const uint8_t data_type = 0x52;
        const uint8_t param = 0x00;
        uint8_t checksum{};
    } Request_Yaw;

    typedef struct _yaw_response
    {
        const uint16_t header = 0xFFFD;
        const uint8_t length = 0XFD;
        const uint8_t data_type = 0x52;
        // Unit is radian and has been scaled to 1000 times, divide by 1000 to get the real value
        // then times 57.29 to get the degree
        uint16_t yaw{};
        uint8_t checksum{};
    } Yaw_Response;

    // Request and response for servo position, data_type = 0x53, param = servo id,
    // response with am uint8_t for servo id and a uint16_t for position.
    typedef struct _request_servo_position
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x06;
        const uint8_t function = 0x20;
        const uint8_t data_type = 0x53;
        // 1-250
        uint8_t servo_id{};
        uint8_t checksum{};
    } Request_Servo_Position;

    typedef struct _servo_position_response
    {
        const uint16_t header = 0xFFFD;
        const uint8_t length = 0XFD;
        const uint8_t data_type = 0x20;
        // 1-250
        uint8_t servo_id{};
        // 96-4000
        uint16_t position{};
        uint8_t checksum{};
    } Servo_Position_Response;

    // Request and response for movement status, data_type = 0x08, param=0x00
    // response with uint8_t for linear velocity, uint16_t for angular velocity,
    // uint16 for x acceleration, uint16_t for y acceleration, uint16_t for z acceleration
    // uint16 for x gyro, uint16_t for y gyro, uint16_t for z gyro
    // uint8 for battery voltage
    typedef struct _request_movement_status
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x50;
        const uint8_t data_type = 0x08;
        const uint8_t param = 0x00;
        uint8_t checksum{};
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
    } Movement_Status_Response;

    // Request and response for PID parameters, data_type = 0x01, param = 0x00
    // response with uint16_t for P, uint16_t for I, uint16_t for D

    typedef struct _request_pid_parameters
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x50;
        const uint8_t data_type = 0x01;
        const uint8_t param = 0x00;
        uint8_t checksum{};
    } Request_PID_Parameters;

    typedef struct _pid_parameters_response
    {
        const uint16_t header = 0xFFFD;
        const uint8_t length = 0XFD;
        const uint8_t data_type = 0x01;
        // 0-10000
        // 1000 times of the real value
        uint16_t P{};
        uint16_t I{};
        uint16_t D{};
        uint8_t checksum{};
    } PID_Parameters_Response;

    // Request and response for whether the current gyroscope assists with adjustment
    // data_type = 0x0C, param = 0x00
    // Response with uint8_t for 0 or 1

    typedef struct _request_gyro_assist
    {
        const uint16_t header = 0xFFFE;
        const uint8_t length = 0x05;
        const uint8_t function = 0x50;
        const uint8_t data_type = 0x0C;
        const uint8_t param = 0x00;
        uint8_t checksum{};
    } Request_Gyro_Assist;

    typedef struct _gyro_assist_response
    {
        const uint16_t header = 0xFFFD;
        const uint8_t length = 0X04;
        const uint8_t data_type = 0x0C;
        // 0 or 1
        uint8_t gyro_assist{};
        uint8_t checksum{};
    } Gyro_Assist_Response;

    enum SEND_FUNCTION : uint8_t
    {
        SET_PID = 0x01,
        SET_CHASSIS_MOTION = 0x02,
        SET_PWM_SERVO = 0x03,
        SET_LED_STRIP = 0x04,
        SET_STRIP_EFFECT = 0x05,
        SET_BEEP = 0x06,
        SET_LIGHT = 0x07,
        SET_AUTO_REPORT_DATA = 0x08,
        SET_PWM_MOTOR = 0x09,
        SET_MIN_VELOCITY = 0x0B,
        SET_GYRO_ENABLE = 0x0C,
        SET_MOTOR_FORWARD = 0x0D,
        SET_ARM_SERVO = 0x20,
        SET_SERVO_ID = 0x21,
        SET_ARM_SERVO_TORQUE = 0x22,
        SET_ARM_MOTION = 0x23,
        SEND_REQUEST = 0x50,
        CLEAR_FLASH = 0xA0
    };
    enum RECEIVE_FUNCTION : uint8_t
    {
        FIRMWARE_VERSION = 0x51,
        YAW_ANGLE = 0x52,
        ARM_SERVO_POSITION = 0x20,
        MOTION_STATUS = 0x08,
        PID_PARAM = 0x01,
        GYRO_ASSIST_ENABLED = 0x0C

    };
    enum Direction : uint8_t
    {
        RECEIVE = 0xFD,
        SEND = 0xFE
    };

    static const std::unordered_map<uint8_t, uint8_t> SEND_PACKAGE_LEN
            {
                    {SEND_FUNCTION::SET_PID,              0x0A},
                    {SEND_FUNCTION::SET_CHASSIS_MOTION,   0x06},
                    {SEND_FUNCTION::SET_PWM_SERVO,        0x05},
                    {SEND_FUNCTION::SET_LED_STRIP,        0x07},
                    {SEND_FUNCTION::SET_STRIP_EFFECT,     0x06},
                    {SEND_FUNCTION::SET_BEEP,             0x05},
                    {SEND_FUNCTION::SET_LIGHT,            0x04},
                    {SEND_FUNCTION::SET_AUTO_REPORT_DATA, 0x04},
                    {SEND_FUNCTION::SET_PWM_MOTOR,        0x06},
                    {SEND_FUNCTION::SET_MIN_VELOCITY,     0x06},
                    {SEND_FUNCTION::SET_GYRO_ENABLE,      0x05},
                    {SEND_FUNCTION::SET_MOTOR_FORWARD,    0x04},
                    {SEND_FUNCTION::SET_ARM_SERVO,        0x08},
                    {SEND_FUNCTION::SET_SERVO_ID,         0x04},
                    {SEND_FUNCTION::SET_ARM_SERVO_TORQUE, 0x04},
                    {SEND_FUNCTION::SET_ARM_MOTION,       0x09},
                    {SEND_FUNCTION::SEND_REQUEST,         0x05},
                    {SEND_FUNCTION::CLEAR_FLASH,          0x04}
            };
    // A set for valid send function
    static const std::unordered_set<uint8_t> VALID_SEND_FUNCTION
            {
                    SEND_FUNCTION::SET_PID,
                    SEND_FUNCTION::SET_CHASSIS_MOTION,
                    SEND_FUNCTION::SET_PWM_SERVO,
                    SEND_FUNCTION::SET_LED_STRIP,
                    SEND_FUNCTION::SET_STRIP_EFFECT,
                    SEND_FUNCTION::SET_BEEP,
                    SEND_FUNCTION::SET_LIGHT,
                    SEND_FUNCTION::SET_AUTO_REPORT_DATA,
                    SEND_FUNCTION::SET_PWM_MOTOR,
                    SEND_FUNCTION::SET_MIN_VELOCITY,
                    SEND_FUNCTION::SET_GYRO_ENABLE,
                    SEND_FUNCTION::SET_MOTOR_FORWARD,
                    SEND_FUNCTION::SET_ARM_SERVO,
                    SEND_FUNCTION::SET_SERVO_ID,
                    SEND_FUNCTION::SET_ARM_SERVO_TORQUE,
                    SEND_FUNCTION::SET_ARM_MOTION,
                    SEND_FUNCTION::SEND_REQUEST,
                    SEND_FUNCTION::CLEAR_FLASH
            };
    // A set for valid receive function
    static const std::unordered_set<uint8_t> VALID_RECEIVE_FUNCTION
            {
                    RECEIVE_FUNCTION::FIRMWARE_VERSION,
                    RECEIVE_FUNCTION::YAW_ANGLE,
                    RECEIVE_FUNCTION::ARM_SERVO_POSITION,
                    RECEIVE_FUNCTION::MOTION_STATUS,
                    RECEIVE_FUNCTION::PID_PARAM,
                    RECEIVE_FUNCTION::GYRO_ASSIST_ENABLED
            };
    static const std::unordered_map<uint8_t, uint8_t> RECEIVE_PACKAGE_LEN
            {
                    {RECEIVE_FUNCTION::FIRMWARE_VERSION,    0x05},
                    {RECEIVE_FUNCTION::YAW_ANGLE,           0x05},
                    {RECEIVE_FUNCTION::ARM_SERVO_POSITION,  0x06},
                    {RECEIVE_FUNCTION::MOTION_STATUS,       0x13},
                    {RECEIVE_FUNCTION::PID_PARAM,           0x09},
                    {RECEIVE_FUNCTION::GYRO_ASSIST_ENABLED, 0x04}
            };
    typedef uint8_t FUNCTION_TYPE;

    class Package
    {
    public:
        Package();

        Package(Direction direction, FUNCTION_TYPE function);

        Package(Package const &aPackage);

        ~Package();

        /**
         * @brief Set data to package
         * The data should be a pointer to the entire package data, not just the parameters data
         * @param data_to_set The data to set
         * @return
         */
        bool set_data(uint8_t *data_to_set);

        bool is_data_set() const;

        uint8_t get_length() const;

        uint8_t *get_data_ptr();

        Direction get_direction() const;

        FUNCTION_TYPE get_function() const;

    private:
        void calculate_checksum();

        Direction m_direction;
        FUNCTION_TYPE m_function;
        uint8_t *data;
        uint8_t checksum;
        uint8_t length;
        bool data_set;
        std::mutex data_mutex;
    };

    const int MAX_PACKAGE_LEN = 0x13;
    const int CIRCLE_BUFFER_SIZE = 10;
}
#endif //TRANSBOT_PACKAGES_HPP
