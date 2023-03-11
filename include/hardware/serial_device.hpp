//
// Created by Charlie on 10/03/2023.
//

#ifndef TRANSBOT_SDK_SERIAL_DEVICE_HPP
#define TRANSBOT_SDK_SERIAL_DEVICE_HPP

#include "hardware_interface.hpp"
#include <string>

namespace transbot_sdk
{

    class SerialDevice : public HardwareInterface
    {
    public:
        explicit SerialDevice(std::string port_name = "/dev/ttyTHS1", int baudrate = 115200);
        ~SerialDevice();
        bool Init();
        int Read(uint8_t *buffer, int len) override;
        int Write(const uint8_t *buffer, int len) override;
    private:
        std::string port_name;
        int serial_file_descriptor;
        bool OpenDevice();
    };

} // transbot_sdk

#endif //TRANSBOT_SDK_SERIAL_DEVICE_HPP
