#Add an executable target to be build from the source files
add_executable(njl5513r njl5513r.cpp)

#Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(njl5513r 
        pico_stdlib 
        hardware_gpio
        hardware_adc
        ) 

#Enable USB output, disable UART output
pico_enable_stdio_usb(njl5513r 1)
pico_enable_stdio_uart(njl5513r 0)

#Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(njl5513r)