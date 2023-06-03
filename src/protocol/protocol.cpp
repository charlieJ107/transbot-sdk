#include <thread>
#include "protocol.hpp"
#include "glog/logging.h"
#include "hardware/serial_device.hpp"
#include <iostream>

Protocol::Protocol()
{
    m_hardware = std::make_shared<transbot_sdk::SerialDevice>();
    m_is_running = false;
    m_receive_buffer_ptr = new uint8_t[transbot_sdk::MAX_PACKAGE_LEN];
    m_receive_buffer =
            std::unordered_map<
                    transbot_sdk::RECEIVE_FUNCTION,
                    std::shared_ptr<CircularBuffer<transbot_sdk::Package>>>();
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
    LOG(INFO) << "Start receive thread.";
    m_receive_thread = std::thread(&Protocol::receive_thread, this);
    // m_receive_thread.join();
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
    if (transbot_sdk::VALID_SEND_FUNCTION.find(package->get_function().send_function) ==
        transbot_sdk::VALID_SEND_FUNCTION.end())
    {
        LOG(ERROR) << "Package is not a valid send package. It has a wrong function.";
        return false;
    }
    // Check package is a valid length
    if (transbot_sdk::SEND_PACKAGE_LEN.at(package->get_function().send_function) != package->get_length())
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

    if (sent_bytes <= 0 || sent_bytes != package->get_length())
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
    if (m_receive_buffer.find(receive_function) == m_receive_buffer.end() ||
        m_receive_buffer.at(receive_function)->is_empty())
    {
        LOG(ERROR) << "Receive buffer is empty.";
        return nullptr;
    }
    // Get a package from the receive buffer
    std::shared_ptr<transbot_sdk::Package> package;
    try
    {
        package = std::make_shared<transbot_sdk::Package>(m_receive_buffer.at(receive_function)->pop());
    }
    catch (std::out_of_range &e)
    {
        LOG(ERROR) << "Receive buffer is empty.";
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
    if (m_receive_thread.joinable())
    {
        LOG(INFO) << "Join receive thread.";
        m_receive_thread.join();
    }
    LOG(INFO) << "Delete receive buffer.";
    delete[] m_receive_buffer_ptr;
}

void Protocol::receive_thread()
{
    LOG(INFO) << "Receive thread started.";
    uint8_t *single_buffer = new uint8_t;
    while (m_is_running)
    {
        memset(m_receive_buffer_ptr, 0, transbot_sdk::MAX_PACKAGE_LEN);
        
        int receive = 0;
        while (true)
        {
            receive = m_hardware->receive(single_buffer, 1);
            if (receive <= 0)
            {
                continue;
            }
            if (*single_buffer == (uint8_t) 0xFF)
            {
                // Find the header
                std::cout << "Receive header." << std::endl;
                m_receive_buffer_ptr[0] = *single_buffer;
                receive = m_hardware->receive(single_buffer, 1);
                if (receive <= 0)
                {
                    continue;
                }

                if (*single_buffer == (uint8_t) 0xFD)
                {
                    // Find the second header
                    std::cout << "Receive second header." << std::endl;
                    m_receive_buffer_ptr[1] = *single_buffer;
                    break;
                }
                else
                {
                    std::cout << "0x" << std::hex << (int) *single_buffer << " " << std::endl;
                    continue;
                }
            }
            std::cout << "0x" << std::hex << (int) *single_buffer << " is not a valid header" << std::endl;
        }
        
        receive = m_hardware->receive(m_receive_buffer_ptr + 2, transbot_sdk::MAX_PACKAGE_LEN - 2);
        if (receive >= 0)
        {

            LOG(INFO) << "Receive another " << receive << " bytes from hardware.";
            for (int i = 0; i < receive + 2; i++)
            {
                std::cout << "0x" << std::hex << (int) m_receive_buffer_ptr[i] << " ";
            }
            std::cout << std::endl;

            // Get function type from the buffer[3]
            auto receive_function = static_cast<transbot_sdk::RECEIVE_FUNCTION>(m_receive_buffer_ptr[3]);
            // Check function type is valid
            auto it = transbot_sdk::VALID_RECEIVE_FUNCTION.find(receive_function);
            if (it == transbot_sdk::VALID_RECEIVE_FUNCTION.end())
            {
                LOG(ERROR) << "Receive function is not valid.";
                continue;
            }
            else
            {
                receive_function = *it;
            }
            // Parse the package
            transbot_sdk::Package package = transbot_sdk::Package(receive_function);
            package.set_data(m_receive_buffer_ptr);
            // Check receive buffer exists, if not, create one
            auto buffer = m_receive_buffer.find(receive_function);
            if (buffer == m_receive_buffer.end())
            {
                LOG(INFO) << "Create a new receive buffer for function: " << receive_function;
                buffer = m_receive_buffer.emplace(
                        receive_function, std::make_shared<CircularBuffer<transbot_sdk::Package>>(10)).first;
            }
            // Push the package to the received buffer
            LOG(INFO) << "Push a package to the receive buffer.";
            buffer->second->push(package);
        }
        else
        {
            LOG(WARNING) << "Invalid data received from hardware, returned bytes: " << receive << ".";
        }
    }
    delete single_buffer;
}
