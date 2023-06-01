#include <thread>
#include "protocol.hpp"
#include "glog/logging.h"
#include "hardware/serial_device.hpp"

Protocol::Protocol()
{
    m_hardware = std::make_shared<transbot_sdk::SerialDevice>();
    m_is_running = false;
    m_receive_buffer_ptr = new uint8_t[transbot_sdk::MAX_PACKAGE_LEN];
    m_receive_buffer =
            std::unordered_map<
                    transbot_sdk::RECEIVE_FUNCTION,
                    CircularBuffer<std::shared_ptr<transbot_sdk::Package>>
            >();
}

bool Protocol::init()
{
    m_is_running = true;
    if (!m_hardware->init())
    {
        LOG(FATAL) << "Hardware init failed.";
        return false;
    }
    // Start a thread to receive data from hardware
    m_receive_thread = std::thread(&Protocol::receive_thread, this);
    return true;
}

bool Protocol::send(const std::shared_ptr<transbot_sdk::Package> &package)
{
    // Check package is a send package
    if (package->get_direction() != transbot_sdk::SEND)
    {
        LOG(ERROR) << "Package is not a send package.";
        return false;
    }
    // Check package is a valid function
    if (transbot_sdk::VALID_SEND_FUNCTION.find(package->get_function()) == transbot_sdk::VALID_SEND_FUNCTION.end())
    {
        LOG(ERROR) << "Package is not a valid send package. It has a wrong function.";
        return false;
    }
    // Check package is a valid length
    if (transbot_sdk::SEND_PACKAGE_LEN.at(package->get_function()) != package->get_length())
    {
        LOG(ERROR) << "Package is not a valid send package. It has a wrong length.";
        return false;
    }
    // Check package data is set
    if (!package->is_data_set())
    {
        LOG(ERROR) << "Package data is not set.";
        return false;
    }
    size_t sent_bytes = m_hardware->send(package->get_data_ptr(), package->get_length());

    if (sent_bytes<=0 || sent_bytes != package->get_length())
    {
        LOG(ERROR) << "Package is not sent completely.";
        return false;
    }
    return true;
}

std::shared_ptr<transbot_sdk::Package> Protocol::take(transbot_sdk::RECEIVE_FUNCTION receive_function)
{
    // Check receive function is valid
    if (transbot_sdk::VALID_RECEIVE_FUNCTION.find(receive_function) == transbot_sdk::VALID_RECEIVE_FUNCTION.end())
    {
        LOG(ERROR) << "Receive function is not valid.";
        return nullptr;
    }
    // Check receive buffer is empty
    if (m_receive_buffer.find(receive_function) != m_receive_buffer.end() ||
        m_receive_buffer.at(receive_function).is_empty())
    {
        LOG(ERROR) << "Receive buffer is empty.";
        return nullptr;
    }
    // Get a package from the receive buffer
    std::shared_ptr<transbot_sdk::Package> package;
    try
    {
        package = m_receive_buffer.at(receive_function).pop();
    }
    catch (std::length_error &e)
    {
        LOG(ERROR) << "Receive buffer is empty.";
        return nullptr;
    }
    // Check package is a received package
    if (package->get_direction() != transbot_sdk::RECEIVE)
    {
        LOG(ERROR) << "Package is not a receive package.";
        return nullptr;
    }
    // Check package is a valid function
    if (package->get_function() != receive_function)
    {
        LOG(ERROR) << "Package is not a valid receive package. It has a wrong function.";
        return nullptr;
    }
    // Check package is a valid length
    if (package->get_length() != transbot_sdk::RECEIVE_PACKAGE_LEN.at(receive_function))
    {
        LOG(ERROR) << "Package is not a valid receive package. It has a wrong length.";
        return nullptr;
    }
    // Check package data is set
    if (!package->is_data_set())
    {
        LOG(ERROR) << "Package data is not set.";
        return nullptr;
    }
    return package;
}

Protocol::~Protocol()
{
    m_is_running = false;
    m_receive_thread.join();
    delete[] m_receive_buffer_ptr;
}

void Protocol::receive_thread()
{
    while (m_is_running)
    {
        auto receive = m_hardware->receive(m_receive_buffer_ptr, transbot_sdk::MAX_PACKAGE_LEN);
        if (receive)
        {
            auto function_type = transbot_sdk::VALID_RECEIVE_FUNCTION.find(m_receive_buffer_ptr[3]);
            if (function_type ==
                transbot_sdk::VALID_RECEIVE_FUNCTION.end())
            {
                LOG(ERROR) << "Invalid function type received from hardware, returned bytes: " << receive << ".";
                continue;
            }

            // Parse package
            auto package =
                    std::make_shared<transbot_sdk::Package>(transbot_sdk::RECEIVE, *function_type);
            package->set_data(m_receive_buffer_ptr);
            // Push package to receive buffer
            try
            {

                m_receive_buffer.at(static_cast<const transbot_sdk::RECEIVE_FUNCTION>(*function_type)).push(package);
            }
            catch (std::length_error &e)
            {
                LOG(ERROR) << "Receive buffer is full.";
            }

        }
        else
        {
            LOG(WARNING) << "Invalid data received from hardware, returned bytes: " << receive << ".";
        }
    }

}


