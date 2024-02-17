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
    static absolute_time_t up_edge_time;
    static absolute_time_t dn_edge_time;
    static inline uint64_t dtime = 0;
    HCSR04(GPIO<Out> out_pin, GPIO<In> in_pin);
    Length<Unit::m> HCSR04::read();          
};

}

#endif // SC19_PICO_HCSR04_HPP_
