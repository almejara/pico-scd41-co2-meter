cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(airQuality C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(airQuality)

include_directories (mates )
include_directories (mates/includes )

target_sources(airQuality PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/scd4x_i2c.c
	${CMAKE_CURRENT_LIST_DIR}/sensirion_common.c
	${CMAKE_CURRENT_LIST_DIR}/sensirion_i2c_hal.c
	${CMAKE_CURRENT_LIST_DIR}/sensirion_i2c.c
	${CMAKE_CURRENT_LIST_DIR}/mates/controller.c
        )

pico_enable_stdio_usb(airQuality 0)
pico_enable_stdio_uart(airQuality 1)

pico_add_extra_outputs(airQuality)

target_link_libraries(airQuality 
    pico_stdlib
    hardware_i2c
    hardware_gpio
)
