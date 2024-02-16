#include "BME280_BBM.hpp" //クラス定義

namespace sc 
{

BME280::BME280(const I2C& i2c)
    _i2c(i2c)
{
    // stdio_init_all();
    // i2c_init(I2C_PORT,100000);
    // gpio_set_function(I2C_SDA,GPIO_FUNC_I2C);
    // gpio_set_function(I2C_SCL,GPIO_FUNC_I2C);

    // gpio_pull_up(I2C_SDA);
    // gpio_pull_up(I2C_SCL);
    int8_t rslt;
    uint32_t delay;               // calculated delay between measurements
    // init_hw();
    rslt = init_sensor(&dev,&delay);
    if (rslt != BME280_OK) {
    //   printf("could not initialize sensor. RC: %d\n", rslt);
        throe Error(__FILE__, __LINE__, "BMEno era-desu");
    }
}

//main()入れただけで何もわかってない(init_hwはいらなそうだからコメントアウトした)
int BME280::get_BME280(){

    printf("Temperature, Pressure, Humidity\n");
    if (read_sensor(&dev,&delay,&sensor_data) == BME280_OK) {
    print_data(&sensor_data);
    sleep_ms(1000*UPDATE_INTERVAL);//UPDATE_INTERVALはCMakeListにあるよ
    }
    else{
    throw Error(__FILE__, __LINE__, "The value could not be read properly.");  // 正常に値が読み取れませんでした
    }
}


int8_t BME280::user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
//   int8_t rslt = 0;
//   asm volatile("nop \n nop \n nop");
//   asm volatile("nop \n nop \n nop");
//   i2c_write_blocking(I2C_PORT,BME280_I2C_ADDR_PRIM,&reg_addr,1,true);
//   sleep_ms(10);
//   i2c_read_blocking(I2C_PORT,BME280_I2C_ADDR_PRIM,reg_data,len,false);
//   asm volatile("nop \n nop \n nop");
//   asm volatile("nop \n nop \n nop");
    Binary read_data = _i2c.read_memory(size_t(len), SlaveAddr(BME280_I2C_ADDR_PRIM), MemoryAddr(reg_addr));
    dead_data.to_assign(reg_data);
    return 0;
}

int8_t BME280::user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
//   int8_t rslt = 0;
//   asm volatile("nop \n nop \n nop");
//   asm volatile("nop \n nop \n nop");
//   uint8_t data_with_reg_addr[len + 1];  // レジスタアドレスとデータを含む配列を作成
//   data_with_reg_addr[0] = reg_addr;  // 先頭にレジスタアドレスを配置
//   for (int i = 0; i < len; i++) {
//     data_with_reg_addr[i + 1] = reg_data[i];  // 残りのデータを配置
//   }
//   i2c_write_blocking(I2C_PORT, BME280_I2C_ADDR_PRIM, data_with_reg_addr, len + 1, false);  // レジスタアドレスとデータを一度に送信
//   asm volatile("nop \n nop \n nop");
//   asm volatile("nop \n nop \n nop");
    _i2c.write_memory(Binary(reg_data, len), SlaveAddr(BME280_I2C_ADDR_PRIM), MemoryAddr(reg_addr));
  return 0;
}

int8_t BME280::init_sensor(struct bme280_dev *dev, uint32_t *delay){
  int8_t rslt = BME280_OK;
  uint8_t settings;

  // give sensor time to startup
  sleep_ms(5);                    // datasheet: 2ms

  //ここを追加した.user_の関数は別のファイルで定義してある
  dev->intf_ptr = I2C_PORT;  // あとで考える
  dev->intf     = BME280_I2C_INTF;
  dev->read     = user_i2c_read;
  dev->write    = user_i2c_write;
  dev->delay_us = [](uint32_t period, void*)->void{sleep_us(period);};
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

int8_t BME280::read_sensor(struct bme280_dev *dev, uint32_t *delay,struct bme280_data *data)
{
  int8_t rslt;
  rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
  if (rslt != BME280_OK) {
    return rslt;
  }
  dev->delay_us(*delay,dev->intf_ptr);
  return bme280_get_sensor_data(BME280_ALL,data,dev);  
}

void BME280::print_data(struct bme280_data *data){
  float temp, press, hum;
  float alt_fac = pow(1.0-ALTITUDE_AT_LOC/44330.0, 5.255);

  temp  = 0.01f * data->temperature;
  press = 0.01f * data->pressure/alt_fac;
  hum   = 1.0f / 1024.0f * data->humidity;
  printf("%0.1f deg C, %0.0f hPa, %0.0f%%\n", temp, press, hum);
}

}
