# Transbot SDK C++ API

This is the C++ API for [Yahboom Transbot](https://category.yahboom.net/products/transbot-jetson_nano), which is a smart robot based on NVIDIA Jetson Nano. It is equipped with a 720P HD camera, 4-DOF robotic arm, and 4-DOF robot steering gear. It supports face recognition, color tracking, object recognition, visual servoing, and other functions. It is a good choice for learning AI and robotics.

This SDK is refactor from the Original Python SDK, and it is compatible with the original Python SDK. You can use this SDK to develop your own C++ application for Transbot.

## Prerequisites

- [Google Glog](https://github.com/google/glog)
  For Ubuntu, you can install it by `sudo apt install libgoogle-glog-dev`

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build
```

## Usage

This SDK use Glog for logging, so you need to initialize Glog before using this SDK.

```cpp
#include <glog/logging.h>

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);

    // ...
}
```

Initialize the Transbot SDK.
```cpp
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
```

Then you can call the API to control the Transbot.

```cpp
// Get firmware version
LOG(INFO) << "firmware version: " << sdk.get_firmware_version();
// Get motion info
auto motion_info = sdk.get_motion_info();
LOG(INFO) << "motion info: \n" <<
            "angular_velocity: " << motion_info.angular_velocity << ", \n" <<
            "linear_velocity: " << motion_info.linear_velocity << ", \n" <<
            "x acc: " << motion_info.x_acceleration << ", \n" <<
            "y acc: " << motion_info.y_acceleration << ", \n" <<
            "z acc: " << motion_info.z_acceleration << ", \n" <<
            "x gyro: " << motion_info.x_gyro << ", \n" <<
            "y gyro: " << motion_info.y_gyro << ", \n" <<
            "z gyro: " << motion_info.z_gyro << ", \n" <<
            "battery voltage: " << motion_info.battery_voltage << ", \n";
```

## API
See [API Reference](API.md)

## License
This SDK is licensed under the [MPL 2.0](https://www.mozilla.org/en-US/MPL/2.0/) license. See [LICENSE](LICENSE) for more details.