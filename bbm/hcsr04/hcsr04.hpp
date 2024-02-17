#ifndef SC19_PICO_HCSR04_HPP_
#define SC19_PICO_HCSR04_HPP_

#include <cstdio>
#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "sc.hpp"

namespace sc
{
//! @brief HCSR04のクラス
//! @param trig_pin 出力用のTrigピン
//! @param echo_pin 入力用のEchoピン
class HCSR04 {
    const GPIO<Out> _out_pin;
    const GPIO<In> _in_pin;
public:
    //! @brief HCSR04のクラス
    //! @param trig_pin 出力用のTrigピン
    //! @param echo_pin 入力用のEchoピン
    HCSR04(Pin trig_pin, Pin echo_pin);
    
    Length<Unit::m> read();          
};

}

#endif // SC19_PICO_HCSR04_HPP_
