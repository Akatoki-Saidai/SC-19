//近距離フェーズのプログラムを書くところです
#include "fm.hpp"

namespace sc 
{

void short_fase(const BME280& bme280, const BNO055& bno055, const HCSR04& hcsr04, const NJL5513R& njl5513r, const GPIO<In>& para_separate, const GPIO<In>& usb_conect, const Motor2& motor, const Speaker& speaker, const LED& led_green, const LED& led_red, const LED& led_pico)
{
    if(true)//ゴールがカメラの真ん中
    {
        //少し進む
         motor.forward(1.0);
         sleep(1_s);
         motor.forward(0);
        //超音波でゴール検知　→ゴール    
        double hscr_date_average,i;
        for(i=0;i<10;++i)//超音波の値の平均
        {
            hscr_date_average += hcsr04.read();
        }
         hscr_date_average/=10.0;
         if(hcsr_date_average<0.2)//0.2m以内でゴール
         {
            print("goal\n");
            while(true)
            {
                ;
            }
         }
    }
    else//角度調整
    {
        break;
    }
}

}