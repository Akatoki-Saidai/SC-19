//落下フェーズのプログラムを書くところです

#include "fm.hpp"

namespace sc 
{

void fall_fase(const BME280& bme280, const BNO055& bno055, const HCSR04& hcsr04, const NJL5513R& njl5513r, const GPIO<In>& para_separate, const GPIO<In>& usb_conect, const Motor2& motor, const Speaker& speaker, const LED& led_green, const LED& led_red, const LED& led_pico)
{
    int separate;
    Altitude<Unit::m> altitude(pressure, temperature);
    if(altitude<1.0)//地面からの標高が１m以内
    {
        sleep(5_s);//念のため待機しておく
        separete=para_separate.read;
        if(separate!=1)//パラシュートが取れていない場合、動いてみる？
        {
         motor.forward(1.0);
         sleep(1_s);
         motor.forward(0);
         break;

        }
         Acceleration<Unit::m_s2> gravity_acceleration = std::get<1>(bno_data);//重力加速度取得
        if(gravity_acceleration.z()>0)//Z軸の重力加速度が正かどうかで機体の体制修正
        {

        }

         fase=Fase::Ldistance;
         break;
    }
    else
    {
         break;
    }
}

}