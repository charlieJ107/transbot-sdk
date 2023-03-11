//
// Created by Charlie on 10/03/2023.
//

#include <unistd.h>
#include <fcntl.h>
#include <glog/logging.h>
#include "hardware/serial_device.hpp"

#ifdef WIN64

#include "windows.h"

#else
#include <termios.h>
#endif

namespace transbot_sdk
{
    bool SerialDevice::Init()
    {

    }

    bool SerialDevice::OpenDevice()
    {
#ifdef __arm__
        serial_file_descriptor = open(port_name.c_str(), O_RDWR | O_NONBLOCK);
#elif __x86_64__
#ifdef WIN64
        serial_file_descriptor = open(port_name.c_str(), O_RDWR);
#else
        serial_file_descriptor = open(port_name.c_str(), O_RDWR | O_NOCTTY);
#endif
#else
        serial_file_descriptor = open(port_name.c_str(), O_RDWR | O_NOCTTY);
#endif
    }
} // transbot_sdk