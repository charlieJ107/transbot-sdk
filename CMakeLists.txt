cmake_minimum_required(VERSION 3.10)
project(transbot_sdk)

set(CMAKE_CXX_STANDARD 14)

find_package(glog CONFIG REQUIRED)

add_library(transbot_sdk
        SHARED
        src/transbot_sdk.cpp
        src/hardware/serial_device.cpp
        src/protocol/protocol.cpp
        src/protocol/package.cpp
        src/protocol/memory_pool.cpp)

target_include_directories(transbot_sdk PUBLIC include)
target_include_directories(transbot_sdk PRIVATE src)
target_link_libraries(transbot_sdk PRIVATE glog::glog)

add_executable(example example/src/main.cpp)
target_link_libraries(example PUBLIC transbot_sdk)
target_link_libraries(example PRIVATE glog::glog)