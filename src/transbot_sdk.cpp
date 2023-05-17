#include "transbot_sdk.hpp"

#include "protocol/packages.hpp"

namespace transbot_sdk
{
    Transbot::Transbot()
    {
        this->protocol = Protocol();
    }

    Transbot::~Transbot()
    {

    }

    bool Transbot::init()
    {
        return this->protocol.init();
    }

    void Transbot::set_chassis_motion(uint8_t linear_velocity, uint16_t angular_velocity)
    {
        protocol.send();
    }
}
