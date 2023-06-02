#include <glog/logging.h>
#include <termios.h>
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

        if (tcgetattr(serial_file_descriptor, &serial_port_settings) != 0)
        {
            LOG(FATAL) << "Get serial port settings failed.";
            return false;
        }

        // Set baud rate to 115200
        cfsetispeed(&serial_port_settings, B115200);
        cfsetospeed(&serial_port_settings, B115200);

        // Set data bits to 8
        serial_port_settings.c_cflag &= ~CSIZE;
        serial_port_settings.c_cflag |= CS8;
        // Allow receve and use local
        serial_port_settings.c_cflag |= (CLOCAL | CREAD);
        // Set parity (奇偶校验) to none
        serial_port_settings.c_cflag &= ~PARENB;
        // Set stop bits to 1
        serial_port_settings.c_cflag &= ~CSTOPB;

        // Set wait time to 40ms
        serial_port_settings.c_cc[VTIME] = 4;
        // Set minimum receive bytes to 4
        serial_port_settings.c_cc[VMIN] = 4;

        // Using raw mode
        serial_port_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        serial_port_settings.c_oflag &= ~OPOST;

        // Flush the input and output buffer
        tcflush(serial_file_descriptor, TCIFLUSH);

        // Set the new options for the port
        if (tcsetattr(serial_file_descriptor, TCSANOW, &serial_port_settings) != 0)
        {
            LOG(FATAL) << "Set serial port settings failed.";
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

        for (int i = 0; i < read_bytes; i++)
        {
            LOG(INFO) << "Receive: " << std::hex << buffer[i];
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