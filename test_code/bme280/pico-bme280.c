// --------------------------------------------------------------------------
// Read BME280 sensor values with a Raspberry Pi Pico using the official Bosch-API
//
// Author: Bernhard Bablok
//
// https://github.com/bablokb/pico-bme280
// --------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "user.h"
#include "bme280.h"

//I2Cに書き換えた
void init_hw(){
  stdio_init_all();
  i2c_init(I2C_PORT,100000);
  gpio_set_function(I2C_SDA,GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL,GPIO_FUNC_I2C);

  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);
}
// ---------------------------------------------------------------------------
// initialize sensor

int8_t init_sensor(struct bme280_dev *dev, uint32_t *delay) {
  int8_t rslt = BME280_OK;
  uint8_t settings;

  // give sensor time to startup
  sleep_ms(5);                    // datasheet: 2ms

  //ここを追加した.user_の関数は別のファイルで定義してある
  dev->intf_ptr = I2C_PORT;
  dev->intf     = BME280_I2C_INTF;
  dev->read     = user_i2c_read;
  dev->write    = user_i2c_write;
  dev->delay_us = user_delay_us;
  rslt = bme280_init(dev);

#ifdef DEBUG
  printf("[DEBUG] chip-id: 0x%x\n",dev->chip_id);
#endif
  if (rslt != BME280_OK) {
    return rslt;
  }
  // configure for forced-mode, indoor navigation
  dev->settings.osr_h = BME280_OVERSAMPLING_1X;
  dev->settings.osr_p = BME280_OVERSAMPLING_16X;
  dev->settings.osr_t = BME280_OVERSAMPLING_2X;
  dev->settings.filter = BME280_FILTER_COEFF_16;

  settings = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
  if (rslt != BME280_OK) {
    return rslt;
  }
  rslt   = bme280_set_sensor_settings(settings,dev);
  *delay = bme280_cal_meas_delay(&dev->settings);
  #ifdef DEBUG
    printf("[DEBUG] delay: %u µs\n",*delay);
  #endif
  return rslt;
}

// ---------------------------------------------------------------------------
// read sensor values

int8_t read_sensor(struct bme280_dev *dev, uint32_t *delay,
                   struct bme280_data *data) {
  int8_t rslt;
  rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
  if (rslt != BME280_OK) {
    return rslt;
  }
  dev->delay_us(*delay,dev->intf_ptr);
  return bme280_get_sensor_data(BME280_ALL,data,dev);
}

// ---------------------------------------------------------------------------
// print sensor data to console

void print_data(struct bme280_data *data) {
  float temp, press, hum;
  float alt_fac = pow(1.0-ALTITUDE_AT_LOC/44330.0, 5.255);

  temp  = 0.01f * data->temperature;
  press = 0.01f * data->pressure/alt_fac;
  hum   = 1.0f / 1024.0f * data->humidity;
  printf("%0.1f deg C, %0.0f hPa, %0.0f%%\n", temp, press, hum);
}

// ---------------------------------------------------------------------------
// main loop: read data and print data to console

int main() {
  // try
  // {
    struct bme280_dev dev;
    struct bme280_data sensor_data;
    int8_t rslt;
    uint32_t delay;               // calculated delay between measurements
    init_hw();
    rslt = init_sensor(&dev,&delay);
    if (rslt != BME280_OK) {
      printf("could not initialize sensor. RC: %d\n", rslt);
    } else {
      printf("Temperature, Pressure, Humidity\n");
      while (read_sensor(&dev,&delay,&sensor_data) == BME280_OK) {
        print_data(&sensor_data);
        sleep_ms(1000*UPDATE_INTERVAL);
      }
      printf("error while reading sensor: RC: %d", rslt);
    }
    return 0;
   //}
  // catch (const std::exception& e)
  // {
  //   printf("error");
  //   sleep_ms(5000);
  //   printf("error");
  // }
}
