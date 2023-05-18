#ifndef TRANSBOT_SDK_PROTOCOL_HPP
#define TRANSBOT_SDK_PROTOCOL_HPP

#include <mutex>
#include <memory>
#include "package.hpp"
#include "../hardware/hardware_interface.hpp"
#include "memory_pool.hpp"
#include "circular_buffer.hpp"

/**
 * @brief Protocol layer for transbot
 * @details This layer is in_use to extract data from the hardware layer and send data to the hardware layer.
 *         It is also in_use to manage the memory pool and circular buffer.
 */
class Protocol
{
public:
    Protocol() = default;

    ~Protocol();

    bool init();

    bool send(const std::shared_ptr<transbot_sdk::Package> &package);

    std::shared_ptr<transbot_sdk::Package> take(transbot_sdk::RECEIVE_FUNCTION receive_function);

private:
    std::shared_ptr<transbot_sdk::HardwareInterface> m_hardware;
//    std::mutex m_mutex;
};

#endif //TRANSBOT_SDK_PROTOCOL_HPP
