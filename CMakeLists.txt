cmake_minimum_required(VERSION 3.24)
project(transbot_sdk)

set(CMAKE_CXX_STANDARD 17)

find_package(glog REQUIRED)

add_library(transbot_sdk
        SHARED
        src/transbot_sdk.cpp
        include/protocol/packages.hpp
        include/hardware/hardware_interface.hpp
        src/hardware/serial_device.cpp
        include/hardware/serial_device.hpp
        )

target_link_libraries(transbot_sdk PUBLIC ${GLOG_LIBRARY})
target_include_directories(transbot_sdk PUBLIC include)

add_executable(example example/src/main.cpp)
target_link_libraries(example transbot_sdk)
target_link_libraries(example PUBLIC ${GLOG_LIBRARY})