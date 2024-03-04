# ライブラリを作成
add_library(SC STATIC)
target_sources(SC PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/adc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/binary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/flush.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gpio.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/i2c_slave.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/i2c.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/measurement.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/motor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/pin.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/pwm.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sc_basic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/spi_slave.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/spi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/uart.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/unit.cpp
)

# インクルードディレクトリを指定
target_include_directories(SC PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# コンパイルする際にライブラリをリンク
target_link_libraries(SC PUBLIC 
    hardware_gpio
    hardware_adc
    hardware_flash
    hardware_i2c
    hardware_pwm
    hardware_spi
    hardware_sync
    hardware_uart
    pico_stdlib
)

# 以下の資料を参考にしました
# https://qiita.com/kikochan/items/732e46e92e7f29c18ce9
# https://qiita.com/shohirose/items/45fb49c6b429e8b204ac
# https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico