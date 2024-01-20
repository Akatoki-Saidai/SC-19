cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(hcsr04_original C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(hcsr04_original
  hcsr04_original.cpp
)

target_link_libraries(hcsr04_original pico_stdlib hardware_gpio)

pico_enable_stdio_usb(hcsr04_original 1)
pico_enable_stdio_uart(hcsr04_original 1)
pico_add_extra_outputs(hcsr04_original)
target_link_libraries(hcsr04_original pico_stdlib)