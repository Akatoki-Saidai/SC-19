//待機フェーズのプログラムを書くところです

#include "fm.hpp"

namespace sc 
{

void wait_fase(const BME280& bme280, const BNO055& bno055, const HCSR04& hcsr04, const NJL5513R& njl5513r, const GPIO<In>& para_separate, const GPIO<In>& usb_conect, const Motor2& motor, const Speaker& speaker, const LED& led_green, const LED& led_red, const LED& led_pico)
{
    auto njl_data = njl5513r.read();
    //照度によりキャリア展開検知　→落下フェーズへ
    if(njl_date>500)//値はテキトウ
     {
         fase=Fase::Fall;
         break;
     }
     else
     {
         break;
     } 
}

}