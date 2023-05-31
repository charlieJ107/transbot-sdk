#ifndef TRANSBOT_TRANSBOT_SDK_HPP
#define TRANSBOT_TRANSBOT_SDK_HPP

#include <string>
#include "data.hpp"
#include "../src/protocol/protocol.hpp"
#include "glog/logging.h"


namespace transbot_sdk
{
    class Transbot
    {
    public:
        Transbot()
        {
            if (!google::IsGoogleLoggingInitialized())
            {
                google::InitGoogleLogging("transbot_sdk");
            }
        }

        ~Transbot() = default;

        /**
         * @brief Initialize the transbot sdk
         * @return true if success
         */
        bool init();

        /**
         * @brief Set the chassis motion
         * @param linear_velocity -45-45
         * @param angular_velocity -200-200
         */
        void set_chassis_motion(int linear_velocity, int angular_velocity);

        /**
         * @brief Set the camara servo angle
         * @param channel 0x01: horizontal, 0x02: vertical, if using depth camera, 0x01: depth
         * @param angle 50-130
         */
        void set_camara_angle(transbot_sdk::TRANSBOT_CAMARA_CHANNEL channel, int angle);

        /**
         * @brief Set the led strip color
         * @param id the id of the led, 0-16, 0xff for all
         * @param r Red, 0-255
         * @param g Green, 0-255
         * @param b Blue, 0-255
         */
        void set_led_strip(int id, int r, int g, int b);

        /**
         * @brief Set the led strip effect
         * @param effect Effect id, 0-6
         * @param velocity 1-10, bigger for faster
         * @param param Effect parameter, 0-6
         * @note Effect id: 0: static, 1: blink, 2: breath, 3: color wipe, 4: color wipe random, 5: random color, 6: single dynamic
         */
        void set_strip_effect(int effect, int velocity, int param);

        /**
         * @brief Set the buzzer beep
         * @param duration 0 for stop, 1 for continue, >1 for duration (shutdown after duration * 10ms)
         */
        void set_beep(int duration);

        /**
         * @brief Set the lightness of the led strip
         * @param lightness 0-100
         */
        void set_light(int lightness);

        /**
         * @brief Toggle the gyro assist, if the angular velocity is 0, the gyro assist can help the robot to move straight
         * @param enable
         */
        void enable_gyro_assist(bool enable);

        /**
         * @brief Move the robot straight
         * @param speed -45-45
         */
        void move_straight(int speed);

        /**
         * @brief Toggle the servo torque
         * @param enable  true for enable, false for disable
         */
        void enable_servo_torque(bool enable);

        /**
         * @brief Set the servo angle
         * @param servoId 0x07: joint1, 0x08: joint2, 0x09: joint3
         * @param angle 0-180
         * @param speed 0 for max speed, bigger for slower
         */
        void set_single_arm_servo_angle(TRANSBOT_ARM_SERVO_ID servoId, int angle, int speed = 100);

        /**
         * @brief Set the servo angle
         * @param joint1  0-225
         * @param joint2 30-270
         * @param joint3 30-180
         * @param speed 0 for max speed, bigger for slower
         */
        void set_all_arm_servo_angle(int joint1, int joint2, int joint3, int speed);

        /**
         * @brief Get the firmware version
         * @return firmware version
         */
        std::string get_firmware_version();

        /**
         * @brief Get yaw angle in radian
         * @return yaw angle in radian
         */
        int get_yaw_angle();

        /**
         * @brief Get the servo position
         * @return The servo position
         */
        int get_servo_position(int channel);

        /**
         * @brief Get chassis motion info
         * @return The motion info
         */
        Motion_Info get_motion_info();

        /**
         * @brief Get current chassis PID parameters
         * @return The PID parameters
         */
        PID_Parameters get_pid_parameters();

        /**
         * @brief Get gyro assist status
         * @return true if gyro assist is enabled
         */
        bool is_gyro_assist_enabled();


    private:
        Protocol protocol;
        int angle_offset[3] = {0, 0, 0};

        uint16_t angle_to_pwm(int angle, TRANSBOT_ARM_SERVO_ID servoId);
    };
} // transbot_sdk

#endif //TRANSBOT_TRANSBOT_SDK_HPP
