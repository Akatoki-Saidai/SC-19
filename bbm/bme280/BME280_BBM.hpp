#ifndef SC19_PICO_BME280_HPP_
#define SC19_PICO_BME280_HPP_

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "user.h"
#include "bme280.h"

// Class declaration
class BME280 {
public:
    BME280();
    int get_BME280();
};

#endif // SC19_PICO_BME280_HPP_