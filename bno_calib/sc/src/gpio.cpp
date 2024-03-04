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

GPIO<Out>::GPIO(Pin pin, Pull pull) try :
    _pin(pin)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    try
    {
        if (Pin::Status.at(_pin.gpio()) == PinStatus::NoUse)
        {
            Pin::Status.at(_pin.gpio()) = PinStatus::Gpio;
        } else {
throw std::logic_error(f_err(__FILE__, __LINE__, "Pin %hhu is already in use", _pin.gpio()));  // このピンは既に使用されています
        }

        ::gpio_init(_pin.gpio());  // pico-SDKの関数  ピンの初期化を行う
        ::gpio_set_dir(_pin.gpio(), GPIO_OUT);  // pico-SDKの関数  通信方向の設定を行う
        _pin.set_pull(pull);  // プルアップ・ダウン抵抗を設定
    }
    catch(const std::exception& e)
    {
        save = false;
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
    }
}
catch (const std::exception& e)
{
    print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
}

void GPIO<Out>::write(bool level) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (save == false)
        throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot execute because initialization failed"));
    ::gpio_put(_pin.gpio(), level);  // pico-SDKの関数  ピンにHighかLowを出力する
}

void GPIO<Out>::on() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (save == false)
        throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot execute because initialization failed"));
    this->write(1);
}

void GPIO<Out>::off() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (save == false)
        throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot execute because initialization failed"));
    this->write(0);
}


/***** class GPIO<In> *****/

GPIO<In>::GPIO(Pin pin):
    GPIO::GPIO(pin, Pull::No) {}

GPIO<In>::GPIO(Pin pin, Pull pull) try :
    _pin(pin)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    try
    {
        if (Pin::Status.at(_pin.gpio()) == PinStatus::NoUse)
        {
            Pin::Status.at(_pin.gpio()) = PinStatus::Gpio;
        } else {
throw std::logic_error(f_err(__FILE__, __LINE__, "Pin %hhu is already in use", _pin.gpio()));  // このピンは既に使用されています
        }

        ::gpio_init(_pin.gpio());  // pico-SDKの関数  ピンの初期化を行う
        ::gpio_set_dir(_pin.gpio(), GPIO_IN);  // pico-SDKの関数  通信方向の設定を行う
        _pin.set_pull(pull);  // プルアップ・ダウン抵抗を設定
    }
    catch(const std::exception& e)
    {
        save = false;
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
    }
}
catch (const std::exception& e)
{
    print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
}

bool GPIO<In>::read() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (save == false)
        throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot execute because initialization failed"));
    return ::gpio_get(_pin.gpio());  // pico-SDKの関数  ピンがHighになっているかLowになっているかを取得する
}

}