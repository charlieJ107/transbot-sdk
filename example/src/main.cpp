#include <glog/logging.h>
#include "transbot_sdk.hpp"

int main(int argc, char *argv[])
{
    FLAGS_logtostderr = true;
    FLAGS_colorlogtostderr = true;
    FLAGS_stderrthreshold = 0;
    google::InitGoogleLogging(argv[0]);
    // google::EnableLogCleaner(0);
    transbot_sdk::Transbot sdk;
    if (sdk.init())
    {
        LOG(INFO) << "init success";
    }
    else
    {
        LOG(ERROR) << "init failed";
        return -1;
    }
    // Wait for packages to be received 5 second
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Get firmware version
    // LOG(INFO) << "firmware version: " << sdk.get_firmware_version();
    // Get motion info
    auto motion_info = sdk.get_motion_info();
    LOG(INFO) << "motion info: " <<
              "angular_velocity: " << motion_info.angular_velocity << ", \n" <<
              "linear_velocity: " << motion_info.linear_velocity << ", \n" <<
              "x acc: " << motion_info.x_acceleration << ", \n" <<
              "y acc: " << motion_info.y_acceleration << ", \n" <<
              "z acc: " << motion_info.z_acceleration << ", \n" <<
              "x gyro: " << motion_info.x_gyro << ", \n" <<
              "y gyro: " << motion_info.y_gyro << ", \n" <<
              "z gyro: " << motion_info.z_gyro << ", \n" <<
              "battery voltage: " << motion_info.battery_voltage << ", \n";

    // Set arm position
    // sdk.set_all_arm_servo_angle(90, 90, 90, 1000);
    // Get arm position:
    auto joint1 = sdk.get_servo_position(transbot_sdk::TRANSBOT_ARM_SERVO_ID::JOINT1);
    auto joint2 = sdk.get_servo_position(transbot_sdk::TRANSBOT_ARM_SERVO_ID::JOINT2);
    auto joint3 = sdk.get_servo_position(transbot_sdk::TRANSBOT_ARM_SERVO_ID::JOINT3);
    LOG(INFO) << "arm position: " << joint1 << ", " << joint2 << ", " << joint3;

    // Move straight
    // sdk.move_straight(15);
    // // Delay 1 second
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // // Stop
    // sdk.move_straight(0);
    // // Move back
    // sdk.move_straight(-15);
    // // Delay 1 second
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // // Stop
    // sdk.move_straight(0);

    // Beep 1 second
    sdk.set_beep(1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sdk.set_beep(0);
    

    return 0;
}