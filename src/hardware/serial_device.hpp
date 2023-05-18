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

        bool init() override;

        size_t receive(uint8_t* buffer, size_t max_length)  override;

        size_t send(uint8_t* buffer, size_t length) override;

    private:
        std::string port_name;
        int baud_rate;
        int serial_file_descriptor;
        struct termios serial_port_settings;
        int max_retry_times;

        bool open_device();

        bool configure_device();
    };
} // transbot_sdk

#endif //TRANSBOT_SDK_SERIAL_DEVICE_HPP
