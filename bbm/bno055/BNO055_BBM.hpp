#ifndef SC19_PICO_BNO055_HPP_
#define SC19_PICO_BNO055_HPP_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Class declaration
class BNO055 {
public:
    BNO055();
    int get_BNO055();          
};

#endif // SC19_PICO_BNO055_HPP_