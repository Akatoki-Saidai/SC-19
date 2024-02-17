#ifndef SC19_PICO_SC_GPIO_HPP_
#define SC19_PICO_SC_GPIO_HPP_

/**************************************************
 * GPIOピンの出入力に関するコードです
 * このファイルは，gpio.cppに書かれている関数の一覧です
 * 
 * このファイルでは，GPIOのセットアップ，GPIOでの出入力に関するコードが宣言されています．
**************************************************/

//! @file gpio.hpp
//! @brief GPIOピンの出入力

#include "sc_basic.hpp"

#include "hardware/gpio.h"

namespace sc
{
//! @brief 入力/出力 (Input/Output)
enum IO
{
    Out,  // 出力
    In  // 入力
};

template<IO> class GPIO;

//! @brief ピンによる出力を行うクラス
template<>
class GPIO<Out>
{
    const Pin _pin;  //! 使用するピンのGPIO番号
public:
    //! @brief 出力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    GPIO(Pin pin);

    //! @brief 出力用ピンをセットアップ
    //! @param pin 出力用ピンのGPIO番号
    //! @param pull プルアップ/プルダウンを指定
    GPIO(Pin pin, Pull pull);

    //! @brief 出力用ピンに書き込み
    //! @param level high(1)かlow(0)か
    void write(bool level) const;

    //! @brief 出力用ピンど電圧をhighにする
    void on() const;

    //! @brief 出力用ピンど電圧をlowにする
    void off() const;

    //! @brief GPIO番号を取得
    //! @return GPIO番号
    uint8_t gpio() const
        {return _pin.gpio();}
};

using LED = GPIO<Out>;  // LEDによる出力

//! @brief ピンによる入力を行うクラス
template<>
class GPIO<In>
{
    const Pin _pin;  //! 使用するピンのGPIO番号
public:
    //! @brief 入力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    GPIO(Pin pin);

    //! @brief 入力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    //! @param pull プルアップ/プルダウンを指定
    GPIO(Pin pin, Pull pull);

    //! @brief 入力用ピンから読み込み
    //! @return High(1)かLow(0)か
    bool read() const;

    //! @brief GPIO番号を取得
    //! @return GPIO番号
    uint8_t gpio() const
        {return _pin.gpio();}
};

}

#endif  // SC19_PICO_SC_gpIO_HPP_