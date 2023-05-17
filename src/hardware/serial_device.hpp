#ifndef TRANSBOT_SDK_SERIAL_DEVICE_HPP
#define TRANSBOT_SDK_SERIAL_DEVICE_HPP

#include "hardware_interface.hpp"
#include <string>
#include <fstream>
#include <termios.h>

namespace transbot_sdk
{
    class SerialDevice : public HardwareInterface
    {
    public:
        explicit SerialDevice(const std::string &port_name = "/dev/ttyTHS1", int baud_rate = 115200);

        ~SerialDevice() override;

        bool Init() override;

        std::vector<uint8_t> Read() override;

        int Write(std::vector<uint8_t> buffer) override;

    private:
        std::string port_name;
        int baud_rate;
        int serial_file_descriptor;
        struct termios serial_port_settings;

        bool OpenDevice();

        bool ConfigureDevice();
    };
} // transbot_sdk

#endif //TRANSBOT_SDK_SERIAL_DEVICE_HPP
