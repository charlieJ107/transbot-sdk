#include <glog/logging.h>

#include <fcntl.h>
#include "serial_device.hpp"

namespace transbot_sdk
{
    bool SerialDevice::Init()
    {
        if (OpenDevice())
        {
            LOG(INFO) << "Open serial device " << this->port_name << " successfully.";
        } else
        {
            LOG(FATAL) << "Open serial device " << this->port_name << " failed.";
            return false;
        }
        return true;
    }

    bool SerialDevice::OpenDevice()
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

    bool SerialDevice::ConfigureDevice()
    {
        if (tcgetattr(serial_file_descriptor, &this->serial_port_settings) != 0)
        {
            LOG(ERROR) << "Get serial port settings failed.";
            return false;
        }
    }


    SerialDevice::SerialDevice(const std::string &port_name, int baud_rate)
    {
        this->port_name = port_name;
        this->baud_rate = baud_rate;
        this->serial_file_descriptor = -1;
    }

    std::vector<uint8_t> SerialDevice::Read()
    {
        std::vector<uint8_t> result;
        char temp_buffer[0x13];
        int read_bytes = read(serial_file_descriptor, &temp_buffer, 0x13);
        while (read_bytes)
        {
            result.insert(result.end(), temp_buffer, temp_buffer + 0x13);
            read_bytes = read(serial_file_descriptor, &temp_buffer, 0x13);
        }
        DLOG(INFO) << "Read data from serial device " << this->port_name << " successfully.";
        return result;
    }

    int SerialDevice::Write(const std::vector<uint8_t> buffer)
    {
        int write_bytes = write(serial_file_descriptor, buffer.data(), buffer.size());
        if (write_bytes < 0)
        {
            LOG(ERROR) << "Write data to serial device " << this->port_name << " failed.";
            return -1;
        }
        DLOG(INFO) << "Write data to serial device " << this->port_name << " successfully.";
        return write_bytes;
    }

    SerialDevice::~SerialDevice()
    {
        close(serial_file_descriptor);
    }


} // transbot_sdk