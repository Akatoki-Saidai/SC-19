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

Pin::Pin(int pin_gpio) try :
    _pin_gpio(pin_gpio)
{
    if (pin_gpio < Pin::MinGpio || Pin::MaxGpio < pin_gpio)
    {
        throw std::invalid_argument(f_err(__FILE__, __LINE__, "GPIO number %d is not available", pin_gpio));  // このGPIO番号は利用できません
    }   
}
catch (const std::exception& e)
{
    print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
    print(e.what());
}

uint8_t Pin::gpio() const
{
    return _pin_gpio;
}

Pin::operator uint8_t() const
{
    return _pin_gpio;
}

void Pin::set_pull(Pull pull) const
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
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

}