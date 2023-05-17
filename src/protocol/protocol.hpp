#ifndef TRANSBOT_SDK_PROTOCOL_HPP
#define TRANSBOT_SDK_PROTOCOL_HPP

#include <mutex>
#include <memory>
#include "hardware/hardware_interface.hpp"
#include "packages.hpp"

class Protocol
{
public:
    Protocol()=default;
    ~Protocol();
    bool init();
    bool send(transbot_sdk::WriteFunction function, uint8_t*data, uint8_t length);
    bool take();
private:
    std::shared_ptr<transbot_sdk::HardwareInterface> m_hardware;
//    std::mutex m_mutex;
};
#endif //TRANSBOT_SDK_PROTOCOL_HPP
