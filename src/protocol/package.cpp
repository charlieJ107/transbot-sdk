#include <glog/logging.h>
#include <mutex>
#include "package.hpp"
#include <cstring>

namespace transbot_sdk
{
    Package::Package()
    {
        m_direction = SEND;
        m_function.send_function = CLEAR_FLASH;
        data_set = false;
        checksum = 0;
        length = 0;
        data = nullptr;
    }
    Package::Package(SEND_FUNCTION send_function)
    {
        // Check send_function must be a valid send function
        if (VALID_SEND_FUNCTION.find(send_function) == VALID_SEND_FUNCTION.end())
        {
            LOG(ERROR) << "Invalid send function: " << send_function;
            throw std::invalid_argument("Invalid send function.");
        }
        m_direction = SEND;
        m_function.send_function = send_function;
        data_set = false;
        checksum = 0;
        length = SEND_PACKAGE_LEN.at(send_function)+2;
        data = new uint8_t[length];
        memset(data, 0, length);
        data[0] = 0xFF;
        data[1] = 0xFE;
        data[2] = length-2;
        data[3] = static_cast<uint8_t>(send_function);

    }

    Package::Package(RECEIVE_FUNCTION receive_function)
    {
        // Check receive_function must be a valid receive function
        if (VALID_RECEIVE_FUNCTION.find(receive_function) == VALID_RECEIVE_FUNCTION.end())
        {
            LOG(ERROR) << "Invalid receive function: " << receive_function;
            throw std::invalid_argument("Invalid receive function.");
        }
        m_direction = RECEIVE;
        m_function.receive_function = receive_function;
        data_set = false;
        checksum = 0;
        length = RECEIVE_PACKAGE_LEN.at(receive_function)+2;
        data = new uint8_t[length];
        memset(data, 0, length);
        data[0] = 0xFF; // Header 0
        data[1] = 0xFD; // Header 1
        data[2] = length-2;
        data[3] = static_cast<uint8_t>(receive_function);
    }

    Package::~Package()
    {
        // Since c++0x, delete[] nullptr is allowed.
        if (data != nullptr)
        {
            delete[] data;
        }
    }

    bool Package::set_data(uint8_t *data_to_set)
    {
        if (data_set)
        {
            LOG(ERROR) << "Data has already been set.";
            return false;
        }
        if (m_direction == SEND)
        {
            if (data_to_set[3] != static_cast<uint8_t>(m_function.send_function))
            {
                LOG(ERROR) << "Function mismatch: " << data_to_set[3] << "!=" << m_function.send_function;
                return false;
            }
        }
        else if (m_direction == RECEIVE)
        {
            if (data_to_set[3] != static_cast<uint8_t>(m_function.receive_function))
            {
                LOG(ERROR) << "Function mismatch: " << data_to_set[3] << "!=" << m_function.receive_function;
                return false;
            }
        }
        else
        {
            LOG(ERROR) << "Invalid direction: " << (int)data_to_set[1] << "!=" << m_direction;
            return false;
        }

        memcpy(this->data + 4, data_to_set + 4, length - 4);
        calculate_checksum();
        data_set = true;
        return true;
    }

    bool Package::is_data_set() const
    {
        return data_set;
    }

    uint8_t *Package::get_data_ptr()
    {
        if (data_set)
        {
            return data;
        }
        else
        {
            LOG(ERROR) << "Data has not been set.";
            return nullptr;
        }
    }

    void Package::calculate_checksum()
    {
        // if the mutex is not locked, lock it.
        // std::lock_guard<std::mutex> lock(data_mutex);
        uint8_t cal = 0;
        for (int i = 2; i < length - 1; i++)
        {
            cal += data[i];
        }
        checksum = cal % 256;
        data[length - 1] = checksum;
    }

    uint8_t Package::get_length() const
    {
        return length;
    }

    Direction Package::get_direction() const
    {
        // return m_direction;
        return static_cast<Direction>(m_direction);
    }

    FUNCTION_TYPE Package::get_function() const
    {
        return m_function;
    }

}