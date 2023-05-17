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
        virtual std::vector<uint8_t> Read() = 0;
        virtual int Write(std::vector<uint8_t> buffer) = 0;
        virtual bool Init() = 0;
    };

} // transbot_sdk

#endif //TRANSBOT_SDK_HARDWARE_INTERFACE_HPP
