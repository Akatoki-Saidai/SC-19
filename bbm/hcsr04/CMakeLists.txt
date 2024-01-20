# 以下はこのファイルを親フォルダのCMakeListsから呼び出すとき用

# ビルドを実行するファイルを追加
add_library(HCSR04 STATIC
    ${CMAKE_CURRENT_LIST_DIR}/hcsr04.cpp
)
# 以下の資料を参考にしました
# https://qiita.com/kikochan/items/732e46e92e7f29c18ce9
# https://qiita.com/shohirose/items/45fb49c6b429e8b204ac

# ライブラリの読み込み
target_link_libraries(HCSR04
    hardware_gpio
    pico_stdlib
)

##################################################
##################################################
##################################################

# cmake_minimum_required(VERSION 3.13)
# include(pico_sdk_import.cmake)
# project(hcsr04_original C CXX ASM)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
# pico_sdk_init()
# add_executable(hcsr04_original
#   hcsr04_original.cpp
# )

# target_link_libraries(hcsr04_original pico_stdlib hardware_gpio)

# pico_enable_stdio_usb(hcsr04_original 1)
# pico_enable_stdio_uart(hcsr04_original 1)
# pico_add_extra_outputs(hcsr04_original)
# target_link_libraries(hcsr04_original pico_stdlib)