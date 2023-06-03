#include <glog/logging.h>
#include <mutex>
#include "package.hpp"
#include <cstring>

namespace transbot_sdk
{
    Package::Package()
    {
        m_direction = SEND;
        m_function = CLEAR_FLASH;
        data_set = false;
        checksum = 0;
        length = 0;
        data = nullptr;
    }

    Package::Package(Direction direction, FUNCTION_TYPE function)
    {
        m_direction = direction;
        m_function = function;
        data_set = false;
        checksum = 0;
        length = 0;
        if (direction == SEND)
        {
            if (function == CLEAR_FLASH)
            {
                data = nullptr;
                LOG(WARNING) << "Clear flash command has no effect for safety reason.";
                return;
            }
            // If function is out of range in enum, the following operation with map will throw an out_of_range exception.
            // The map grantees that only the valid function can be found.
            length = SEND_PACKAGE_LEN.at(function);
            data = new uint8_t[length];
            memset(data, 0, length);
            data[0] = 0xFF;
            data[1] = 0xFE;
            data[2] = length;
            data[3] = function;
        }
        else if (direction == RECEIVE)
        {
            // If function is out of range in enum, the following operation with map will throw an out_of_range exception.
            // The map grantees that only the valid function can be found.
            length = RECEIVE_PACKAGE_LEN.at(function);
            data = new uint8_t[length];
            memset(data, 0, length);
            data[0] = 0xFF; // Header 0
            data[1] = 0xFD; // Header 1
            data[2] = length;
            data[3] = function;
        }
        else
        {
            data = nullptr;
            LOG(FATAL) << "Invalid direction: " << direction;
            throw std::invalid_argument("Invalid direction.");
        }
    }

    Package::~Package()
    {
        // Since c++0x, delete[] nullptr is allowed.
        delete[] data;
    }

    bool Package::set_data(uint8_t *data_to_set)
    {
        std::lock_guard<std::mutex> lock(data_mutex);
        if (data_set)
        {
            LOG(ERROR) << "Data has already been set.";
            return false;
        }
        if (data_to_set[3] != m_function)
        {
            LOG(ERROR) << "Function mismatch: " << data_to_set[3] << "!=" << m_function;
            return false;
        }
        if (data_to_set[1] != SEND && data_to_set[1] != RECEIVE || data_to_set[1] != m_direction)
        {
            LOG(ERROR) << "Invalid direction: " << data_to_set[1] << "!=" << m_direction;
            return false;
        }
        memcpy(this->data + 4, data_to_set + 4, length - 5);
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
        std::lock_guard<std::mutex> lock(data_mutex);
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
        std::lock_guard<std::mutex> lock(data_mutex);
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
        return m_direction;
    }

    FUNCTION_TYPE Package::get_function() const
    {
        return m_function;
    }

    Package::Package(Package const &aPackage)
    {
        m_direction = aPackage.m_direction;
        m_function = aPackage.m_function;
        data_set = aPackage.data_set;
        checksum = aPackage.checksum;
        length = aPackage.length;
        data = new uint8_t[length];
        memcpy(data, aPackage.data, length);
    }
}