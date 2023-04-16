//
// Created by Charlie on 10/03/2023.
//

#ifndef TRANSBOT_SDK_HARDWARE_INTERFACE_HPP
#define TRANSBOT_SDK_HARDWARE_INTERFACE_HPP

#include <cstdint>

namespace transbot_sdk
{

    class HardwareInterface
    {
    public:
        HardwareInterface();

        virtual ~HardwareInterface() = default;

    protected:
        virtual int Read(uint8_t *buffer, int len) = 0;

        virtual int Write(const uint8_t *buffer, int len) = 0;

    };

} // transbot_sdk

#endif //TRANSBOT_SDK_HARDWARE_INTERFACE_HPP
