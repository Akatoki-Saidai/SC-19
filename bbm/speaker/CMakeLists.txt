#Add an executable target to be build from the source files
add_executable(speaker speaker.cpp)

#Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(speaker 
    pico_stdlib 
    hardware_gpio
    hardware_pwm
    )

#Enable USB output, disable UART output
pico_enable_stdio_usb(speaker 1)
pico_enable_stdio_uart(speaker 0)

#Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(speaker)