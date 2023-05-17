#include <glog/logging.h>
#include "transbot_sdk.hpp"

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    transbot_sdk::Transbot transbot;

    return 0;
}