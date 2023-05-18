#include "transbot_sdk.hpp"

#include "protocol/package.hpp"

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

}
