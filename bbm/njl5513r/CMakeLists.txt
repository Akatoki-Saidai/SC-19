# 以下はこのファイルを親フォルダのCMakeListsから呼び出すとき用

# ビルドを実行するファイルを追加
add_library(NJL5513R STATIC
    ${CMAKE_CURRENT_LIST_DIR}/njl5513r.cpp
)
# 以下の資料を参考にしました
# https://qiita.com/kikochan/items/732e46e92e7f29c18ce9
# https://qiita.com/shohirose/items/45fb49c6b429e8b204ac

# ライブラリの読み込み
target_link_libraries(NJL5513R
        pico_stdlib 
        hardware_gpio
        hardware_adc
)

##################################################
##################################################
##################################################

# #Add an executable target to be build from the source files
# add_executable(njl5513r njl5513r.cpp)

# #Pull in our pico_stdlib which aggregates commonly used features
# target_link_libraries(njl5513r 
#         pico_stdlib 
#         hardware_gpio
#         hardware_adc
#         ) 

# #Enable USB output, disable UART output
# pico_enable_stdio_usb(njl5513r 1)
# pico_enable_stdio_uart(njl5513r 0)

# #Create map/bin/hex/uf2 file etc.
# pico_add_extra_outputs(njl5513r)