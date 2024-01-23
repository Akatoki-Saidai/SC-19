/**************************************************
 * ピンの使用に関するコードです
 * このファイルは，pin.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，ピンのGPIO番号などに関するコードが定義されています．
**************************************************/

//! @file pin.cpp
//! @brief ピンの使用

#include "sc_basic.hpp"

#include "pin.hpp"

namespace sc
{

/***** class Pin *****/

Pin::Pin(int pin_gpio):
    _pin_gpio(pin_gpio)
{
    if (pin_gpio < Pin::MinGpio || Pin::MaxGpio < pin_gpio)
    {
        throw Error(__FILE__, __LINE__, "This GPIO number is not available");  // このGPIO番号は利用できません
    }
    
}

uint8_t Pin::get_gpio() const
{
    return _pin_gpio;
}

Pin::operator uint8_t() const
{
    return _pin_gpio;
}

void Pin::set_pull(Pull pull) const
{
    switch (pull)
    {
        case Pull::Up:
        {
            ::gpio_pull_up(_pin_gpio);  // pico-SDKの関数  プルアップ抵抗を設定
            break;
        }
        case Pull::Down:
        {
            ::gpio_pull_down(_pin_gpio);  // pico-SDKの関数  プルダウン抵抗を設定
            break;
        }
        case Pull::No:
        {
            ::gpio_disable_pulls(_pin_gpio);  // pico-SDKの関数  プルアップ・プルダウンを無効化
            break;
        }
    }
}

std::vector<PinStatus> Pin::Status(28, PinStatus::NoUse);

}