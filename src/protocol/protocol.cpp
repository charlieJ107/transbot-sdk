#include "protocol.hpp"
#include "glog/logging.h"

bool Protocol::init()
{
    return m_hardware->init();
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

    if (sent_bytes == -1 || sent_bytes != package->get_length())
    {
        LOG(ERROR) << "Package is not sent completely.";
        return false;
    }
    return true;
}

std::shared_ptr<transbot_sdk::Package> Protocol::take(transbot_sdk::RECEIVE_FUNCTION receive_function)
{
    return {};
}

Protocol::~Protocol()
{

}
