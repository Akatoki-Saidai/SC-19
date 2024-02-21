//落下フェーズのプログラムを書くところです

#include "fm.hpp"

namespace sc 
{

void fall_fase(const BME280& bme280, const BNO055& bno055, const HCSR04& hcsr04, const NJL5513R& njl5513r, const GPIO<In>& para_separate, const GPIO<In>& usb_conect, const Motor2& motor, const Speaker& speaker, const LED& led_green, const LED& led_red, const LED& led_pico)
{
    Altitude<Unit::m> altitude(pressure, temperature);
    if(altitude==0){
        
    }
}

}