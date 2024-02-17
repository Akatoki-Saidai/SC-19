#ifndef SC19_PICO_SC_ADC_HPP_
#define SC19_PICO_SC_ADC_HPP_

/**************************************************
 * ADCによる入力に関するコードです
 * このファイルは，adc.cppに書かれている関数の一覧です
 * 
**************************************************/

//! @file adc.hpp
//! @brief ADCによる入力

#include "sc_basic.hpp"

#include "pwm.hpp"

#include "hardware/adc.h"


namespace sc
{

// ADC(アナログ-デジタル変換)
class ADC
{
public:
    //! @brief PWMのセットアップ
    //! @param pin PWMで出力するためのピン
    ADC(Pin adc_pin);

private:
    const Pin _adc_pin;
    const uint8_t _channel;

    static inline bool IsUse[5] = {false, false, false, false, false};  // 既にADCを使用しているか

    static uint8_t get_channel(Pin pin)
        {return pin.gpio() - 26;}

public:
    //! @brief ADCを読み取り
    uint16_t read() const;

};

}

#endif  // SC19_PICO_SC_ADC_HPP_