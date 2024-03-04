#ifndef SC19_PICO_BNO055_HPP_
#define SC19_PICO_BNO055_HPP_

#include <cmath>
#include <cstdio>
#include <tuple>
#include "sc.hpp"

namespace sc 
{

// Class declaration
class BNO055 {
    const I2C& _i2c;
    void accel_init(void);
public:
    BNO055(const I2C& i2c);
    std::tuple<Acceleration<Unit::m_s2>,Acceleration<Unit::m_s2>,MagneticFluxDensity<Unit::T>,AngularVelocity<Unit::rad_s>> read();          
};

}

#endif // SC19_PICO_BNO055_HPP_