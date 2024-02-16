#ifndef SC19_PICO_HCSR04_HPP_
#define SC19_PICO_HCSR04_HPP_

#include <cstdio>
#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "sc.hpp"

namespace sc
{

// Class declaration
class HCSR04 {
    const GPIO<Out>& _out_pin;
    const GPIO<In>& _in_pin;
public:
    HCSR04(GPIO<Out> out_pin, GPIO<In> in_pin);
    int gettingTime();          
};

}

#endif // SC19_PICO_HCSR04_HPP_
