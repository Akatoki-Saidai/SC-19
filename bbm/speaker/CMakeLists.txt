# 以下はこのファイルを親フォルダのCMakeListsから呼び出すとき用

# ビルドを実行するファイルを追加
add_library(SPEAKER STATIC
    ${CMAKE_CURRENT_LIST_DIR}/speaker.cpp
)
# 以下の資料を参考にしました
# https://qiita.com/kikochan/items/732e46e92e7f29c18ce9
# https://qiita.com/shohirose/items/45fb49c6b429e8b204ac

# ライブラリの読み込み
target_link_libraries(SPEAKER
    pico_stdlib 
    hardware_gpio
    hardware_pwm
    SC
)

##################################################
##################################################
##################################################

# #Add an executable target to be build from the source files
# add_executable(speaker speaker.cpp)

# #Pull in our pico_stdlib which aggregates commonly used features
# target_link_libraries(speaker 
#     pico_stdlib 
#     hardware_gpio
#     hardware_pwm
#     )

# #Enable USB output, disable UART output
# pico_enable_stdio_usb(speaker 1)
# pico_enable_stdio_uart(speaker 0)

# #Create map/bin/hex/uf2 file etc.
# pico_add_extra_outputs(speaker)