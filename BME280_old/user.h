// --------------------------------------------------------------------------
// Declaration of user-supplied functions for Bosch-API
//
// Author: Bernhard Bablok
//
// https://github.com/bablokb/pico-bme280
// --------------------------------------------------------------------------

#ifndef _USER_H
#define _USER_H

#include "pico/stdlib.h"


//ここを追加した
#ifndef I2C_PORT
  #define I2C_PORT i2c1
#endif
#ifndef I2C_SDA
  #define I2C_SDA 18
#endif
#ifndef I2C_SCL
  #define I2C_SCL 19
#endif


void user_delay_us(uint32_t period, void *intf_ptr);
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);

#endif
