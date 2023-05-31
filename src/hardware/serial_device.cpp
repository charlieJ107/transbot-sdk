#include <glog/logging.h>

#include <fcntl.h>
#include "serial_device.hpp"

namespace transbot_sdk
{
    bool SerialDevice::init()
    {
        if (open_device())
        {
            LOG(INFO) << "Open serial device " << this->port_name << " successfully.";
        } else
        {
            LOG(FATAL) << "Open serial device " << this->port_name << " failed.";
            return false;
        }
        return true;
    }

    bool SerialDevice::open_device()
    {
#ifdef __arm__
        this->serial_file_descriptor = open(this->port_name.c_str(), O_RDWR | O_NBLOCK);
#else
        serial_file_descriptor = open(this->port_name.c_str(), O_RDWR | O_NOCTTY);
#endif
        if (serial_file_descriptor < 0)
        {
            LOG(ERROR) << "Open serial device " << this->port_name << " failed.";
            return false;
        }
        return true;
    }

    bool SerialDevice::configure_device()
    {
        if (tcgetattr(serial_file_descriptor, &this->serial_port_settings) != 0)
        {
            LOG(FATAL) << "Get serial port settings failed.";
            return false;
        }
        return true;
    }


    SerialDevice::SerialDevice(const std::string &port_name, int baud_rate)
    {
        this->port_name = port_name;
        this->baud_rate = baud_rate;
        this->serial_file_descriptor = -1;
        this->serial_port_settings = {};
        max_retry_times = 5;
    }

    size_t SerialDevice::receive(uint8_t *buffer, size_t max_length)
    {
        if (buffer == nullptr)
        {
            LOG(ERROR) << "Buffer is nullptr.";
            return -1;
        }
        size_t read_bytes = read(serial_file_descriptor, buffer, max_length);
        while (read_bytes==0){
            LOG(WARNING) << "Connection lost. Try to reconnect.";
            while (!init()){
                LOG(WARNING) << "Reconnect failed. Try again.";
                usleep(1000000);
            }
            LOG(INFO) << "Reconnect successfully.";
            read_bytes = read(serial_file_descriptor, buffer, max_length);
        }
        return read_bytes;
    }

    size_t SerialDevice::send(uint8_t *buffer, size_t length)
    {
        return write(serial_file_descriptor, buffer, length);
    }

    SerialDevice::~SerialDevice()
    {
        close(serial_file_descriptor);
    }


} // transbot_sdk