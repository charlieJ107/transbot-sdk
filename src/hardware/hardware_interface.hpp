//
// Created by Charlie on 10/03/2023.
//

#ifndef TRANSBOT_SDK_HARDWARE_INTERFACE_HPP
#define TRANSBOT_SDK_HARDWARE_INTERFACE_HPP

#include <cstdint>
#include <vector>

namespace transbot_sdk
{

    class HardwareInterface
    {
    public:
        HardwareInterface() = default;

        virtual ~HardwareInterface() = default;

        /**
         * @brief Receive data from hardware
         * @param buffer A created buffer to store received data
         * @param max_length Max length of the buffer
         * @return Length of the received data
         */
        virtual size_t receive(uint8_t *buffer, size_t max_length) = 0;

        /**
         * @brief Send data to hardware
         * @param buffer buffer to send
         * @param length length of the buffer
         * @return Length of the sent data
         */
        virtual size_t send(uint8_t *buffer, size_t length) = 0;

        /**
         * @brief Initialize hardware
         * @return true if success
         */
        virtual bool init() = 0;
    };

} // transbot_sdk

#endif //TRANSBOT_SDK_HARDWARE_INTERFACE_HPP
