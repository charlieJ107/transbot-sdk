cmake_minimum_required(VERSION 3.5)
project(transbot_sdk)

set(CMAKE_CXX_STANDARD 14)
if (DEFINED CMAKE_TOOLCHAIN_FILE)
    find_package(glog REQUIRED)
else ()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(glog REQUIRED libglog)
endif ()


add_library(transbot_sdk
        SHARED
        src/transbot_sdk.cpp
        src/hardware/serial_device.cpp
        src/protocol/protocol.cpp
        src/protocol/package.cpp
        src/protocol/memory_pool.cpp)

add_executable(example example/src/main.cpp)

target_include_directories(transbot_sdk PUBLIC include)
target_include_directories(transbot_sdk PRIVATE src ${glog_INCLUDE_DIRECTORY})

target_link_libraries(transbot_sdk PRIVATE ${glog_LIBRARIES})
target_link_libraries(example PRIVATE ${glog_LIBRARIES})
target_link_libraries(example PUBLIC transbot_sdk)