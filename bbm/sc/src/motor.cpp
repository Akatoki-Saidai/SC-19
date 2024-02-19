/**************************************************
 * モーターの制御に関するコードです
 * このファイルは，motor.hppに名前だけ書かれている関数の中身です
 * 
**************************************************/

//! @file motor.cpp
//! @brief モーターの制御

#include "motor.hpp"

namespace sc
{


void Motor1::run(float speed) const
{
    if (speed < -1.0F || +1.0F < speed) throw Error(__FILE__, __LINE__, "The motor output is a number between 0 and 1. However, %d was entered.", speed);  // スピードが0未満または1以上ならエラー

    if (speed >= 0.0F)
    {
        // 正回転するとき
        _in1_pwm.write(speed);
        _in2_pwm.write(0.0F);
    } else {
        // 逆回転するとき
        _in1_pwm.write(0.0F);
        _in2_pwm.write(-speed);
    }
}

// ブレーキをかける
void Motor1::brake() const
{
    _in1_pwm.write(1.0F);
    _in2_pwm.write(1.0F);
}

}