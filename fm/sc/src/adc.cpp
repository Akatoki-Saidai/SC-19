/**************************************************
 * ADCによる入力に関するコードです
 * このファイルは，adc.hppに名前だけ書かれている関数の中身です
 * 
**************************************************/

//! @file adc.cpp
//! @brief ADCによる入力

#include "adc.hpp"

namespace sc
{

ADC::ADC(Pin adc_pin):
    _adc_pin(adc_pin), _channel(ADC::get_channel(adc_pin))
{
    if (!(Pin::Status.at(_adc_pin.gpio()) == PinStatus::NoUse))
    {
throw Error(__FILE__, __LINE__, "This pin is already in use");  // このピンは既に使用されています
    } else if (ADC::IsUse[_channel]) {
throw Error(__FILE__, __LINE__, "ADC cannot be reinitialized");  // ADCを再度初期化することはできません
    }

    Pin::Status.at(_adc_pin.gpio()) = PinStatus::Adc;

    ADC::IsUse[_channel] = true;

    if (ADC::IsUse[0]==false && ADC::IsUse[1]==false && ADC::IsUse[2]==false && ADC::IsUse[3]==false && ADC::IsUse[4]==false)
    {
        ::adc_init();
    }

    ::adc_gpio_init(_adc_pin.gpio());

    ::adc_select_input(_channel);    
}

uint16_t ADC::read() const
{
    return ::adc_read();
}

}