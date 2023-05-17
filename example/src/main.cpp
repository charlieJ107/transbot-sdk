#include <glog/logging.h>
#include <fcntl.h>
#include "transbot_sdk.hpp"

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    int serial = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial == -1)
    {
        LOG(ERROR) << "Failed to open serial port";
        return -1;
    }
    uint8_t buffer[0x13];
    read(serial, buffer, 0x13);
    LOG(INFO) << "Read: " << buffer;
    close(serial);
    return 0;
}