#ifndef SC19_PICO_NJL5513R_HPP_
#define SC19_PICO_NJL5513R_HPP_

#include "sc.hpp"

namespace sc 
{

class NJL5513R 
{

    const ADC& _lux_adc;
    const LED& _red_led;
    const LED& _green_led;

public:
    NJL5513R(const ADC& lux_adc, const LED& red_led, const LED& green_led):
        _lux_adc(lux_adc), _red_led(red_led), _green_led(green_led) {}

    const LED& green_led()
        {return _green_led;}

    const LED& red_led()
        {return _red_led;}

    uint16_t read() 
        {return _lux_adc.read();}

};

}

#endif  // SC19_PICO_NJL5513R_HPP_