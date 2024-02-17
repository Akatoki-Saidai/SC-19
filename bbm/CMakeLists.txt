# 最低限必要なCMakeのバージョンを設定
cmake_minimum_required(VERSION 3.12)

# プログラミング言語を設定
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pico-SDKのパスを設定 (環境によって違う)
# set(PICO_SDK_PATH "C:/Pico/pico-sdk")

# SDKの読み込み (プロジェクトに関する設定の前にある必要がある)
include(pico_sdk_import.cmake)

# プロジェクトを設定
project(BBM C CXX ASM)

# 例外を有効にする
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)
# 以下の資料を参考にしました
# https://community.element14.com/products/raspberry-pi/b/blog/posts/raspberry-pico-and-cmake---enable-c-exceptions-and-rtti-run-time-type-information

# 警告レベルを上げる
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
endif()
# 以下の資料を参考にしました
# https://theolizer.com/cpp-school1/cpp-school1-7/#w4

# SDKを初期化
pico_sdk_init()

# サブディレクトリを登録
add_subdirectory(sc)
add_subdirectory(bme280)
add_subdirectory(bno055)
add_subdirectory(drv8835)
add_subdirectory(hcsr04)
add_subdirectory(njl5513r)
add_subdirectory(sd)
add_subdirectory(speaker)

# 実行ファイルを作成
add_executable(BBM
    bbm.cpp
)

# コンパイルする際にライブラリをリンク
target_link_libraries(BBM
    pico_stdlib
    SC
    BME280
    BNO055
    DRV8835
    HCSR04
    NJL5513R
    SD
    SPEAKER
)

# USB出力を有効にし，UART出力を無効にする
pico_enable_stdio_usb(BBM 1)
pico_enable_stdio_uart(BBM 0)

# map/bin/hex/uf2などのファイルを追加で出力する
pico_add_extra_outputs(BBM)