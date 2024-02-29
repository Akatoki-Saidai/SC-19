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
    NJL5513R(const ADC& lux_adc, const LED& red_led, const LED& green_led) try :
        _lux_adc(lux_adc), _red_led(red_led), _green_led(green_led) 
    {
        try
        {
        }
        catch(const std::exception& e)
        {
            print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
        }
    }
    catch (const std::exception& e)
    {
        print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
    }

    const LED& green_led()
        {return _green_led;}

    const LED& red_led()
        {return _red_led;}

    Illuminance<Unit::lx> read() 
        {
            uint16_t adc_value[3];
            for (int i=0; i<3; ++i)
            {
                adc_value[i] = _lux_adc.read();
            }
            uint16_t adc_m = median(adc_value[0], adc_value[1], adc_value[2]);
            print("njl_read_data:%f\n", adc_m * 9);  // 9倍して単位がlxになるのは実験値
            return Illuminance<Unit::lx>(adc_m * 9);  // 9倍して単位がlxになるのは実験値
        }

};

/*
ルクス
目安となる状況・値
    100,000
    雪山や真夏の海岸・100,000
    晴天時午前10時と午後3時の太陽光・65,000
    10,000
    曇天の日、日出1時間後の太陽光・2,000
    1,000
    パチンコ店の店内・1,000
    百貨店売場・500~700
    日出・日入の時・300
    夜のアーケード・150~200
    100
    街灯の下・50~100
    10
    市民薄明※1・5
    1
    月明り・0.5~1
    航海薄明※2・0.01
*/

}

#endif  // SC19_PICO_NJL5513R_HPP_