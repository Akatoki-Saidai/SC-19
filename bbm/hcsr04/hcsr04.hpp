#ifndef SC19_PICO_HCSR04_HPP_
#define SC19_PICO_HCSR04_HPP_

#include <cstdio>
#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Class declaration
class HCSR04 {
public:
    HCSR04();
    int gettingTime();          
    int gotTime;
};

#endif // SC19_PICO_HCSR04_HPP_
