#ifndef TRANSBOT_SDK_PROTOCOL_HPP
#define TRANSBOT_SDK_PROTOCOL_HPP

#include <mutex>
#include <memory>
#include <thread>
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

    void receive_thread();

    uint8_t *m_receive_buffer_ptr;
    bool m_is_running = false;
    std::thread m_receive_thread;
    std::unordered_map<transbot_sdk::RECEIVE_FUNCTION, CircularBuffer<std::shared_ptr<transbot_sdk::Package>>> m_receive_buffer;
//    std::mutex m_mutex;
};

#endif //TRANSBOT_SDK_PROTOCOL_HPP
