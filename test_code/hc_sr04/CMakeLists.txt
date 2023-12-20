cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(hcsr04 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(hcsr04
  hcsr04.c
)

target_link_libraries(hcsr04 pico_stdlib hardware_gpio)

pico_enable_stdio_usb(hcsr04 1)
pico_enable_stdio_uart(hcsr04 1)
pico_add_extra_outputs(hcsr04)
target_link_libraries(hcsr04 pico_stdlib)