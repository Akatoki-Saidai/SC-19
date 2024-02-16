# 以下はこのファイルを親フォルダのCMakeListsから呼び出すとき用

# ビルドを実行するファイルを追加
add_library(BME280 STATIC
    ${CMAKE_CURRENT_LIST_DIR}/BME280_BBM.cpp
    #BBM用のファイルを追記
    # ${CMAKE_CURRENT_LIST_DIR}/pico-bme280.c
    ${CMAKE_CURRENT_LIST_DIR}/bme280.c
    # ${CMAKE_CURRENT_LIST_DIR}/user.c
)
# 以下の資料を参考にしました
# https://qiita.com/kikochan/items/732e46e92e7f29c18ce9
# https://qiita.com/shohirose/items/45fb49c6b429e8b204ac

add_compile_definitions(I2C_PORT=i2c1)
add_compile_definitions(I2C_SDA=18)
add_compile_definitions(I2C_SCL=19)
add_compile_definitions(ALTITUDE_AT_LOC=520)

add_compile_definitions(BME280_32BIT_ENABLE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_VERBOSE_MAKEFILE 1)
  add_compile_definitions(DEBUG)
  add_compile_definitions(UPDATE_INTERVAL=10)
else()
  add_compile_definitions(UPDATE_INTERVAL=60)
endif()

# ライブラリの読み込み
target_link_libraries(BME280
    pico_stdlib
    hardware_i2c
)

##################################################
##################################################
##################################################


# ## --------------------------------------------------------------------------
# ## Read BME280 sensor values with a Raspberry Pi Pico using the official Bosch-API
# ##
# ## Author: Bernhard Bablok
# ##
# ## https://github.com/bablokb/pico-bme280
# ## --------------------------------------------------------------------------

# # Generated Cmake Pico project file

# cmake_minimum_required(VERSION 3.13)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)

# # initalize pico_sdk from installed location
# # (note this can come from environment, CMake cache etc)
# #set(PICO_SDK_PATH "/path/to/sdk")

# # Pull in Raspberry Pi Pico SDK (must be before project)
# include(pico_sdk_import.cmake)

# project(pico-bme280 C CXX ASM)
# set(PICO_CXX_ENABLE_EXCEPTIONS 1)
# set(PICO_CXX_ENABLE_RTTI 1)
# # Initialise the Raspberry Pi Pico SDK
# pico_sdk_init()

# # Add executable. Default name is the project name, version 0.1

# add_executable(pico-bme280 pico-bme280.c bme280.c user.c)

# #I2Cに書き換えた
# add_compile_definitions(I2C_PORT=i2c1)
# add_compile_definitions(I2C_SDA=18)
# add_compile_definitions(I2C_SCL=19)
# add_compile_definitions(ALTITUDE_AT_LOC=520)

# add_compile_definitions(BME280_32BIT_ENABLE)

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#   set(CMAKE_VERBOSE_MAKEFILE 1)
#   add_compile_definitions(DEBUG)
#   add_compile_definitions(UPDATE_INTERVAL=10)
# else()
#   add_compile_definitions(UPDATE_INTERVAL=60)
# endif()

# pico_set_program_name(pico-bme280 "pico-bme280")
# pico_set_program_version(pico-bme280 "0.1")

# pico_enable_stdio_uart(pico-bme280 0)
# pico_enable_stdio_usb(pico-bme280 1)

# # Add the standard library to the build
# target_link_libraries(pico-bme280 pico_stdlib)

# # Add any user requested libraries
# target_link_libraries(pico-bme280
#         hardware_i2c
#         )

# pico_add_extra_outputs(pico-bme280)

