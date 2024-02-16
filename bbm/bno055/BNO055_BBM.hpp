#ifndef SC19_PICO_BNO055_HPP_
#define SC19_PICO_BNO055_HPP_

#include <stdio.h>

#include "sc.hpp"

namespace sc 
{

// Class declaration
class BNO055 {
    const I2C& _i2c;
    void accel_init(void);
public:
    BNO055(const I2C& i2c);
    int get_BNO055();          
};

}

#endif // SC19_PICO_BNO055_HPP_