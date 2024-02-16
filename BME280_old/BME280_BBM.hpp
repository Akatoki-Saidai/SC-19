#ifndef SC19_PICO_BME280_HPP_
#define SC19_PICO_BME280_HPP_

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "user.h"
#include "bme280.h"

#include "sc.hpp"

namespace sc 
{

// Class declaration
class BME280 {
    struct bme280_dev dev;
    struct bme280_data sensor_data;
    const I2C& _i2c;
    int8_t init_sensor(struct bme280_dev *dev, uint32_t *delay);
    int8_t read_sensor(struct bme280_dev *dev, uint32_t *delay,struct bme280_data *data);
    void print_data(struct bme280_data *data);
    int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
    int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
public:
    BME280(const I2C& i2c);
    int get_BME280();
};

}

#endif // SC19_PICO_BME280_HPP_