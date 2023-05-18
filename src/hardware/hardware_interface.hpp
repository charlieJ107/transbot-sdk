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
        virtual size_t receive(uint8_t* buffer, size_t max_length) = 0;
        virtual size_t send(uint8_t* buffer, size_t length) = 0;
        virtual bool init() = 0;
    };

} // transbot_sdk

#endif //TRANSBOT_SDK_HARDWARE_INTERFACE_HPP
