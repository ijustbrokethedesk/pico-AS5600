# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-AS5600 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico-AS5600 main.cpp lib/AS5600/AS5600.cpp)

pico_set_program_name(pico-AS5600 "pico-AS5600")
pico_set_program_version(pico-AS5600 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pico-AS5600 1)
pico_enable_stdio_usb(pico-AS5600 1)

# Add the standard library to the build
target_link_libraries(pico-AS5600
        pico_stdlib
        hardware_i2c
)

# Add the standard include files to the build
target_include_directories(pico-AS5600 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/lib
)

pico_add_extra_outputs(pico-AS5600)

