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

/***** class ADC *****/

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

    if (ADC::IsUse[0]==false && ADC::IsUse[1]==false && ADC::IsUse[2]==false && ADC::IsUse[3]==false && ADC::IsUse[4]==false)
    {
        ::adc_init();
    }

    ADC::IsUse[_channel] = true;

    ::adc_gpio_init(_adc_pin.gpio());
}

uint16_t ADC::read() const
{
    ::adc_select_input(_channel);
    return ::adc_read();
}


/***** class PicoTemp *****/

PicoTemp::PicoTemp()
{
    if (ADC::IsUse[0]==false && ADC::IsUse[1]==false && ADC::IsUse[2]==false && ADC::IsUse[3]==false && ADC::IsUse[4]==false)
    {
        ::adc_init();
    }

    ADC::IsUse[4] = true;

    ::adc_set_temp_sensor_enabled(true);  // 温度センサを有効にする
}

dimension::degC PicoTemp::read()
{
    ::adc_select_input(4);
    return dimension::degC(27 - ((::adc_read() * 3.3 / (1<<12)) - 0.706)/0.001721);
}


/***** class VsysVoltage *****/

VsysVoltage::VsysVoltage()
{
    if (ADC::IsUse[0]==false && ADC::IsUse[1]==false && ADC::IsUse[2]==false && ADC::IsUse[3]==false && ADC::IsUse[4]==false)
    {
        ::adc_init();
    }

    ADC::IsUse[3] = true;

    ::adc_gpio_init(29);
}

dimension::V VsysVoltage::read()
{
    ::adc_select_input(3);
    return dimension::V(3 * ::adc_read() * 3.3 / (1 << 12));
}


}