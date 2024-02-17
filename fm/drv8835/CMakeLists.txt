# 以下はこのファイルを親フォルダのCMakeListsから呼び出すとき用

# ビルドを実行するファイルを追加
add_library(DRV8835 STATIC
    ${CMAKE_CURRENT_LIST_DIR}/drv8835.cpp
)
# 以下の資料を参考にしました
# https://qiita.com/kikochan/items/732e46e92e7f29c18ce9
# https://qiita.com/shohirose/items/45fb49c6b429e8b204ac

# ライブラリの読み込み
target_link_libraries(DRV8835
    pico_stdlib 
    hardware_gpio
    hardware_pwm
)

##################################################
##################################################
##################################################

# #Add an executable target to be build from the source files
# add_executable(drv8835 drv8835.cpp)

# #Pull in our pico_stdlib which aggregates commonly used features
# target_link_libraries(drv8835 
#     pico_stdlib 
#     hardware_gpio
#     hardware_pwm
#     )

# #Enable USB output, disable UART output
# pico_enable_stdio_usb(drv8835 1)
# pico_enable_stdio_uart(drv8835 0)

# #Create map/bin/hex/uf2 file etc.
# pico_add_extra_outputs(drv8835)