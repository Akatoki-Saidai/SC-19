// --------------------------------------------------------------------------
// Definition of user-supplied functions for Bosch-API
//
// Author: Bernhard Bablok
//
// https://github.com/bablokb/pico-bme280
// --------------------------------------------------------------------------
#include "user.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
//ここを追加した    
#include "bme280_defs.h"

void user_delay_us(uint32_t period, void *intf_ptr) {
  sleep_us(period);
}


int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  int8_t rslt = 0;
  asm volatile("nop \n nop \n nop");
  asm volatile("nop \n nop \n nop");
  i2c_write_blocking(I2C_PORT,BME280_I2C_ADDR_PRIM,&reg_addr,1,true);
  sleep_ms(10);
  i2c_read_blocking(I2C_PORT,BME280_I2C_ADDR_PRIM,reg_data,len,false);
  asm volatile("nop \n nop \n nop");
  asm volatile("nop \n nop \n nop");
  return rslt;
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  int8_t rslt = 0;
  asm volatile("nop \n nop \n nop");
  asm volatile("nop \n nop \n nop");
  uint8_t data_with_reg_addr[len + 1];  // レジスタアドレスとデータを含む配列を作成
  data_with_reg_addr[0] = reg_addr;  // 先頭にレジスタアドレスを配置
  for (int i = 0; i < len; i++) {
    data_with_reg_addr[i + 1] = reg_data[i];  // 残りのデータを配置
  }
  i2c_write_blocking(I2C_PORT, BME280_I2C_ADDR_PRIM, data_with_reg_addr, len + 1, false);  // レジスタアドレスとデータを一度に送信
  asm volatile("nop \n nop \n nop");
  asm volatile("nop \n nop \n nop");
  return rslt;
}

// int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
//   int8_t rslt = 0;
//   asm volatile("nop \n nop \n nop");
//   asm volatile("nop \n nop \n nop");
//   i2c_write_blocking(I2C_PORT,BME280_I2C_ADDR_PRIM,&reg_addr,1,true);
//   i2c_write_blocking(I2C_PORT,BME280_I2C_ADDR_PRIM,reg_data,len,false);
//   asm volatile("nop \n nop \n nop");
//   asm volatile("nop \n nop \n nop");
//   return rslt;
// }