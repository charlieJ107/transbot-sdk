#ifndef TRANSBOT_SDK_PROTOCOL_HPP
#define TRANSBOT_SDK_PROTOCOL_HPP

#include <mutex>
#include <memory>
#include "hardware/hardware_interface.hpp"

class Protocol
{
public:
    Protocol()=default;
    ~Protocol();
    bool Init();
    bool Send();
    bool Take();
private:
    std::shared_ptr<transbot_sdk::HardwareInterface> m_hardware_interface;
    std::mutex m_mutex;
};
#endif //TRANSBOT_SDK_PROTOCOL_HPP
