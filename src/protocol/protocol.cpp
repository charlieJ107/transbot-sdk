#include "protocol.hpp"

bool Protocol::send(transbot_sdk::WriteFunction function, uint8_t *data, uint8_t length)
{
    // Convert data to a vector
    std::vector<uint8_t> data_vector(data, data + length);
    int write_bytes = m_hardware->Write(data_vector);
    if (write_bytes != length)
    {
        return false;
    }
    return true;
}
