#include "transbot_sdk.hpp"
#include "protocol/package.hpp"
#include "glog/logging.h"

namespace transbot_sdk
{
    bool Transbot::init()
    {
        return this->protocol.init();
    }

    void Transbot::set_chassis_motion(int linear_velocity, int angular_velocity)
    {
        if (linear_velocity < -45 || linear_velocity > 45)
        {
            LOG(ERROR) << "Linear velocity out of range: " << linear_velocity;
            return;
        }
        if (angular_velocity < -200 || angular_velocity > 200)
        {
            LOG(ERROR) << "Angular velocity out of range: " << angular_velocity;
            return;
        }
        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_CHASSIS_MOTION);
        auto data = new Move_Control{
                .linear_velocity = static_cast<uint8_t>(linear_velocity),
                .angular_velocity = static_cast<uint16_t>(angular_velocity)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set chassis motion successfully." << "Linear velocity: " << static_cast<int>(linear_velocity)
                      << "Angular velocity: " << static_cast<int>(angular_velocity);
        }
        else
        {
            LOG(ERROR) << "Set chassis motion failed." << "Linear velocity: " << static_cast<int>(linear_velocity)
                       << "Angular velocity: " << static_cast<int>(angular_velocity);
        }
        delete data;
    }

    void Transbot::set_camara_angle(transbot_sdk::TRANSBOT_CAMARA_CHANNEL channel, int angle)
    {
        if (angle < 50 || angle > 130)
        {
            LOG(ERROR) << "Angle out of range: " << angle;
            return;
        }
        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_PWM_SERVO);
        auto data = new PWM_Servo_Control{
                .servo_id = channel,
                .angle = static_cast<uint8_t>(angle)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set camara angle successfully." << "Channel: " << static_cast<int>(channel)
                      << "Angle: " << angle;
        }
        else
        {
            LOG(ERROR) << "Set camara angle failed." << "Channel: " << static_cast<int>(channel)
                       << "Angle: " << angle;
        }
        delete data;
    }

    void Transbot::set_led_strip(int id, int r, int g, int b)
    {
        if (id != 0xff && (id < 0 || id > 16))
        {
            LOG(ERROR) << "Led id out of range: " << id;
            return;
        }
        if (r < 0 || r > 255)
        {
            LOG(ERROR) << "Red out of range: " << r;
            return;
        }
        if (g < 0 || g > 255)
        {
            LOG(ERROR) << "Green out of range: " << g;
            return;
        }
        if (b < 0 || b > 255)
        {
            LOG(ERROR) << "Blue out of range: " << b;
            return;
        }

        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_LED_STRIP);
        auto data = new RGB_Control{
                .rgb_id = static_cast<uint8_t>(id),
                .r = static_cast<uint8_t>(r),
                .g = static_cast<uint8_t>(g),
                .b = static_cast<uint8_t>(b)
        };

        package->set_data(reinterpret_cast<uint8_t *>(data));

        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set led strip successfully." << "Id: " << id
                      << "R: " << r << "G: " << g << "B: " << b;
        }
        else
        {
            LOG(ERROR) << "Set led strip failed." << "Id: " << id
                       << "R: " << r << "G: " << g << "B: " << b;
        }
        delete data;
    }

    void Transbot::set_strip_effect(int effect, int velocity, int param)
    {
        if (effect < 0 || effect > 6)
        {
            LOG(ERROR) << "Effect out of range: " << effect;
            return;
        }
        if (velocity < 1 || velocity > 10)
        {
            LOG(ERROR) << "Velocity out of range: " << velocity;
            return;
        }

        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_STRIP_EFFECT);
        auto data = new RGB_Effect{
                .effect_type = static_cast<uint8_t>(effect),
                .frequency = static_cast<uint8_t>(velocity),
                .param = static_cast<uint8_t>(param)
        };

        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set led strip effect successfully." << "Effect: " << effect
                      << "Velocity: " << velocity << "Param: " << param;
        }
        else
        {
            LOG(ERROR) << "Set led strip effect failed." << "Effect: " << effect
                       << "Velocity: " << velocity << "Param: " << param;
        }
        delete data;
    }

    void Transbot::set_beep(int duration)
    {
        if (duration < 0 || duration > 255)
        {
            LOG(ERROR) << "Duration out of range: " << duration;
            return;
        }

        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_BEEP);
        auto data = new Buzzer{
                .time = static_cast<uint8_t>(10 * duration)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set beep successfully." << "Duration: " << duration;
        }
        else
        {
            LOG(ERROR) << "Set beep failed." << "Duration: " << duration;
        }
        delete data;

    }

    void Transbot::set_light(int lightness)
    {
        if (lightness < 0 || lightness > 100)
        {
            LOG(ERROR) << "Lightness out of range: " << lightness;
            return;
        }
        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_LIGHT);
        auto data = new LED_Light{
                .light = static_cast<uint8_t>(lightness)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set light successfully." << "Lightness: " << lightness;
        }
        else
        {
            LOG(ERROR) << "Set light failed." << "Lightness: " << lightness;
        }
        delete data;

    }

    void Transbot::enable_gyro_assist(bool enable)
    {
        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_GYRO_ENABLE);
        auto data = new Gyro_Direction{
                .enable = static_cast<uint8_t>(enable ? transbot_sdk::TRANSBOT_ENABLE::ENABLE
                                                      : transbot_sdk::TRANSBOT_ENABLE::DISABLE)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set gyro assist successfully." << "Enable: " << enable;
        }
        else
        {
            LOG(ERROR) << "Set gyro assist failed." << "Enable: " << enable;
        }
        delete data;
    }

    void Transbot::move_straight(int speed)
    {
        if (speed < -45 || speed > 45)
        {
            LOG(ERROR) << "Speed out of range: " << speed;
            return;
        }
        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_MOTOR_FORWARD);
        auto data = new Forward{
                .velocity = static_cast<int8_t>(speed)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Move straight successfully." << "Speed: " << speed;
        }
        else
        {
            LOG(ERROR) << "Move straight failed." << "Speed: " << speed;
        }
        delete data;

    }

    void Transbot::enable_servo_torque(bool enable)
    {
        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_ARM_SERVO_TORQUE);
        auto data = new Enable_Servo_Torque{
                .enable = static_cast<uint8_t>(enable ? transbot_sdk::TRANSBOT_ENABLE::ENABLE
                                                      : transbot_sdk::TRANSBOT_ENABLE::DISABLE)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set servo torque successfully." << "Enable: " << enable;
        }
        else
        {
            LOG(ERROR) << "Set servo torque failed." << "Enable: " << enable;
        }
        delete data;

    }

    void Transbot::set_single_arm_servo_angle(TRANSBOT_ARM_SERVO_ID servoId, int angle, int speed)
    {

        switch (servoId)
        {
            case TRANSBOT_ARM_SERVO_ID::JOINT1:
                if (angle < 0 || angle > 225)
                {
                    LOG(ERROR) << "Angle out of range: " << angle;
                    return;
                }
                break;
            case TRANSBOT_ARM_SERVO_ID::JOINT2:
                if (angle < 30 || angle > 270)
                {
                    LOG(ERROR) << "Angle out of range: " << angle;
                    return;
                }
                break;
            case TRANSBOT_ARM_SERVO_ID::JOINT3:
                if (angle < 30 || angle > 180)
                {
                    LOG(ERROR) << "Angle out of range: " << angle;
                    return;
                }
                break;
            default:
                LOG(ERROR) << "Servo id out of range: " << servoId;
                return;
        }
        if (speed < 0)
        {
            LOG(ERROR) << "Speed out of range: " << speed;
            return;
        }

        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_ARM_SERVO);
        auto data = new Servo_Control{
                .servo_id = static_cast<uint8_t>(servoId),
                // Implicitly cast to uint16_t
                .target_position = static_cast<uint16_t>(angle_to_pwm(angle, servoId)),
                .time = static_cast<uint8_t>(speed)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (this->protocol.send(package))
        {
            LOG(INFO) << "Set single arm servo angle successfully." << "Servo id: " << servoId
                      << "Angle: " << angle;
        }
        else
        {
            LOG(ERROR) << "Set single arm servo angle failed." << "Servo id: " << servoId
                       << "Angle: " << angle;
        }
    }


    uint16_t Transbot::angle_to_pwm(int angle, TRANSBOT_ARM_SERVO_ID servoId)
    {
        switch (servoId)
        {
            case TRANSBOT_ARM_SERVO_ID::JOINT1:
                return (3100 - 900) * (angle - angle_offset[0] - 180) / (-180) + 900;
            case TRANSBOT_ARM_SERVO_ID::JOINT2:
                return (3100 - 900) * (angle - angle_offset[1] - 90 - 180) / (-180) + 900;
            case TRANSBOT_ARM_SERVO_ID::JOINT3:
                return (3100 - 900) * (angle + angle_offset[2]) / 180 + 900;
            default:
                LOG(ERROR) << "Servo id out of range: " << servoId;
                return 100;
        }
    }

    void Transbot::set_all_arm_servo_angle(int joint1, int joint2, int joint3, int speed)
    {
        if (speed < 0)
        {
            LOG(ERROR) << "Speed out of range: " << speed;
            return;
        }
        if (joint1 < 0 || joint1 > 225)
        {
            LOG(ERROR) << "Joint1 angle out of range: " << joint1;
            return;
        }

        if (joint2 < 30 || joint2 > 270)
        {
            LOG(ERROR) << "Joint2 angle out of range: " << joint2;
            return;
        }

        if (joint3 < 30 || joint3 > 180)
        {
            LOG(ERROR) << "Joint3 angle out of range: " << joint3;
            return;
        }

        auto package =
                std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SET_ARM_MOTION);
        auto data = new Control_Arm_Joint_Position{
                // Implicitly cast to uint16_t
                .joint1 = static_cast<uint16_t>(angle_to_pwm(joint1, TRANSBOT_ARM_SERVO_ID::JOINT1)),
                .joint2 = static_cast<uint16_t>(angle_to_pwm(joint2, TRANSBOT_ARM_SERVO_ID::JOINT2)),
                .joint3 = static_cast<uint16_t>(angle_to_pwm(joint3, TRANSBOT_ARM_SERVO_ID::JOINT3)),
                .time = static_cast<uint8_t>(speed)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));

        if (protocol.send(package))
        {
            LOG(INFO) << "Set all arm servo angle successfully." << "Joint1: " << joint1
                      << "Joint2: " << joint2 << "Joint3: " << joint3;
        }
        else
        {
            LOG(ERROR) << "Set all arm servo angle failed." << "Joint1: " << joint1
                       << "Joint2: " << joint2 << "Joint3: " << joint3;
        }
        delete data;
    }

    std::string Transbot::get_firmware_version()
    {
        auto package = std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SEND_REQUEST);
        auto data = new Request_Firmware_Version;
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (protocol.send(package))
        {
            LOG(INFO) << "Get firmware version successfully.";
        }
        else
        {
            LOG(ERROR) << "Get firmware version failed.";
        }
        delete data;
        // Take response
        auto response = protocol.take(FIRMWARE_VERSION);
        if (response == nullptr)
        {
            LOG(ERROR) << "Get firmware version failed.";
            return "";
        }
        auto major = response->get_data_ptr()[4];
        auto minor = response->get_data_ptr()[5];
        std::string version = std::to_string(major) + "." + std::to_string(minor);
        LOG(INFO) << "Firmware version: " << version;
        return version;
    }

    int Transbot::get_yaw_angle()
    {
        auto package = std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SEND_REQUEST);
        auto data = new Request_Yaw;
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (protocol.send(package))
        {
            LOG(INFO) << "Get yaw angle successfully.";
        }
        else
        {
            LOG(ERROR) << "Get yaw angle failed.";
        }
        delete data;
        // Take response
        auto response = protocol.take(YAW_ANGLE);
        if (response == nullptr)
        {
            LOG(ERROR) << "Get yaw angle failed.";
            return -1;
        }
        auto angle = reinterpret_cast<uint16_t *>(response->get_data_ptr()[4]);
        LOG(INFO) << "Yaw angle: " << angle;
        return static_cast<int>(*angle);
    }

    int Transbot::get_servo_position(int channel)
    {
        auto package = std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SEND_REQUEST);
        auto data = new Request_Servo_Position{
                .servo_id = static_cast<uint8_t>(channel)
        };
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (protocol.send(package))
        {
            LOG(INFO) << "Get servo position successfully.";
        }
        else
        {
            LOG(ERROR) << "Get servo position failed.";
        }
        delete data;
        // Take response
        auto response = protocol.take(ARM_SERVO_POSITION);
        if (response == nullptr)
        {
            LOG(ERROR) << "Get servo position failed.";
            return -1;
        }
        auto servo_id = response->get_data_ptr()[4];
        auto position = reinterpret_cast<uint16_t *>(response->get_data_ptr()[5]);
        LOG(INFO) << "Servo id: " << servo_id << "; Position: " << position;

        return static_cast<int>(*position);
    }

    Motion_Info Transbot::get_motion_info()
    {
        auto package = std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SEND_REQUEST);
        auto data = new Request_Movement_Status;
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (protocol.send(package))
        {
            LOG(INFO) << "Get motion info successfully.";
        }
        else
        {
            LOG(ERROR) << "Get motion info failed.";
        }
        delete data;
        // Take response
        auto response = protocol.take(MOTION_STATUS);
        if (response == nullptr)
        {
            LOG(ERROR) << "Get motion info failed.";
            return {};
        }
        auto linear_velocity = reinterpret_cast<int8_t *>(response->get_data_ptr()[4]);
        auto angular_velocity = reinterpret_cast<int16_t *>(response->get_data_ptr()[5]);
        auto acc_x = reinterpret_cast<int16_t *>(response->get_data_ptr()[7]);
        auto acc_y = reinterpret_cast<int16_t *>(response->get_data_ptr()[9]);
        auto acc_z = reinterpret_cast<int16_t *>(response->get_data_ptr()[11]);
        auto gyro_x = reinterpret_cast<int16_t *>(response->get_data_ptr()[13]);
        auto gyro_y = reinterpret_cast<int16_t *>(response->get_data_ptr()[15]);
        auto gyro_z = reinterpret_cast<int16_t *>(response->get_data_ptr()[17]);
        auto battery_voltage = reinterpret_cast<uint8_t *>(response->get_data_ptr()[19]);

        LOG(INFO) << "Linear velocity: " << linear_velocity << "; Angular velocity: " << angular_velocity
                  << "; Acc x: " << acc_x << "; Acc y: " << acc_y << "; Acc z: " << acc_z
                  << "; Gyro x: " << gyro_x << "; Gyro y: " << gyro_y << "; Gyro z: " << gyro_z
                  << "; Battery voltage: " << battery_voltage;

        return Motion_Info{
                .linear_velocity = static_cast<int>(*linear_velocity),
                .angular_velocity = static_cast<int>(*angular_velocity),
                .x_acceleration = static_cast<int>(*acc_x),
                .y_acceleration = static_cast<int>(*acc_y),
                .z_acceleration = static_cast<int>(*acc_z),
                .x_gyro= static_cast<int>(*gyro_x),
                .y_gyro = static_cast<int>(*gyro_y),
                .z_gyro = static_cast<int>(*gyro_z),
                .battery_voltage = static_cast<int>(*battery_voltage)
        };
    }

    PID_Parameters Transbot::get_pid_parameters()
    {
        auto package = std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SEND_REQUEST);
        auto data = new Request_PID_Parameters;
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (protocol.send(package))
        {
            LOG(INFO) << "Get PID parameters successfully.";
        }
        else
        {
            LOG(ERROR) << "Get PID parameters failed.";
        }
        delete data;
        // Take response
        auto response = protocol.take(PID_PARAM);
        if (response == nullptr)
        {
            LOG(ERROR) << "Get PID parameters failed.";
            return {};
        }
        auto P = reinterpret_cast<int16_t *>(response->get_data_ptr()[4]);
        auto I = reinterpret_cast<int16_t *>(response->get_data_ptr()[6]);
        auto D = reinterpret_cast<int16_t *>(response->get_data_ptr()[8]);
        // Divide by 1000 to get the real value
        double p_real = static_cast<double >(*P) / 1000.0;
        double i_real = static_cast<double >(*I) / 1000.0;
        double d_real = static_cast<double >(*D) / 1000.0;
        LOG(INFO) << "P: " << p_real << "; I: " << i_real << "; D: " << d_real;

        return PID_Parameters{
                .P = p_real,
                .I = i_real,
                .D = d_real
        };

    }

    bool Transbot::is_gyro_assist_enabled()
    {
        auto package = std::make_shared<Package>(Direction::SEND, SEND_FUNCTION::SEND_REQUEST);
        auto data = new Request_Gyro_Assist;
        package->set_data(reinterpret_cast<uint8_t *>(data));
        if (protocol.send(package))
        {
            LOG(INFO) << "Get gyro assist status successfully.";
        }
        else
        {
            LOG(ERROR) << "Get gyro assist status failed.";
        }
        delete data;
        // Take response
        auto response = protocol.take(GYRO_ASSIST_ENABLED);
        if (response == nullptr)
        {
            LOG(ERROR) << "Get gyro assist status failed.";
            return false;
        }
        auto status = response->get_data_ptr()[4];
        LOG(INFO) << "Gyro assist status: " << status;
        return status == ENABLE;
    }

}



