#include "gpio.hpp"

/**************************************************
 * GPIOピンの出入力に関するコードです
 * このファイルは，gpio.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，GPIOのセットアップ，GPIOでの出入力に関するコードが定義されています．
**************************************************/

//! @file gpio.cpp
//! @brief GPIOピンの出入力

namespace sc
{

/***** class GPIO *****/

GPIO<Out>::GPIO(Pin pin):
    GPIO::GPIO(pin, Pull::No) {}

GPIO<Out>::GPIO(Pin pin, Pull pull):
    _pin(pin)
{
    if (Pin::Status.at(_pin.get_gpio()) == PinStatus::NoUse)
    {
        Pin::Status.at(_pin.get_gpio()) = PinStatus::Gpio;
    } else {
throw Error(__FILE__, __LINE__, "This pin is already in use");  // このピンは既に使用されています
    }

    ::gpio_init(_pin.get_gpio());  // pico-SDKの関数  ピンの初期化を行う
    ::gpio_set_dir(_pin.get_gpio(), GPIO_OUT);  // pico-SDKの関数  通信方向の設定を行う
    _pin.set_pull(pull);  // プルアップ・ダウン抵抗を設定
}

void GPIO<Out>::write(bool level) const
{
    ::gpio_put(_pin.get_gpio(), level);  // pico-SDKの関数  ピンにHighかLowを出力する
}

void GPIO<Out>::on() const
{
    this->write(1);
}

void GPIO<Out>::off() const
{
    this->write(0);
}


/***** class GPIO<In> *****/

GPIO<In>::GPIO(Pin pin):
    GPIO::GPIO(pin, Pull::No) {}

GPIO<In>::GPIO(Pin pin, Pull pull):
    _pin(pin)
{
    if (Pin::Status.at(_pin.get_gpio()) == PinStatus::NoUse)
    {
        Pin::Status.at(_pin.get_gpio()) = PinStatus::Gpio;
    } else {
throw Error(__FILE__, __LINE__, "This pin is already in use");  // このピンは既に使用されています
    }

    ::gpio_init(_pin.get_gpio());  // pico-SDKの関数  ピンの初期化を行う
    ::gpio_set_dir(_pin.get_gpio(), GPIO_IN);  // pico-SDKの関数  通信方向の設定を行う
    _pin.set_pull(pull);  // プルアップ・ダウン抵抗を設定
}

bool GPIO<In>::read() const
{
    return ::gpio_get(_pin.get_gpio());  // pico-SDKの関数  ピンがHighになっているかLowになっているかを取得する
}

}