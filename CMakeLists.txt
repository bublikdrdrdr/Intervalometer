cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME Intervalometer)
project(${PROJECT_NAME})

#set(${CMAKE_PROJECT_NAME}_SKETCH ${CMAKE_SOURCE_DIR})

file(GLOB ${CMAKE_PROJECT_NAME}_SRCS
        "*.cpp"
        "menu/*.cpp")

file(GLOB ${CMAKE_PROJECT_NAME}_HDRS
        "*.h"
        "menu/*.h")

set(CMAKE_CXX_STANDARD 11)

#### Uncomment below additional settings as needed.
set(${CMAKE_PROJECT_NAME}_BOARD nano)
set(${CMAKE_PROJECT_NAME}_PORT COM6)
set(nano.build.mcu atmega328p)
set(nano.upload.protocol arduino)
set(nano.upload.speed 57600)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
