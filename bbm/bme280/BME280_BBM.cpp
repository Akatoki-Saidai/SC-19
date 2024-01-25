#include "BME280.hpp" //クラス定義


BME280::BME280(){

    // stdio_init_all();
    // i2c_init(I2C_PORT,100000);
    gpio_set_function(I2C_SDA,GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL,GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

//main()入れただけで何もわかってない(init_hwはいらなそうだからコメントアウトした)
int BME280::get_BME280(){
    struct bme280_dev dev;
    struct bme280_data sensor_data;
    int8_t rslt;
    uint32_t delay;               // calculated delay between measurements
    // init_hw();
    rslt = init_sensor(&dev,&delay);
    if (rslt != BME280_OK) {
      printf("could not initialize sensor. RC: %d\n", rslt);
    }
    else {
      printf("Temperature, Pressure, Humidity\n");
      if (read_sensor(&dev,&delay,&sensor_data) == BME280_OK) {
        print_data(&sensor_data);
        sleep_ms(1000*UPDATE_INTERVAL);
      }
      else{
      throw Error(__FILE__, __LINE__, "The value could not be read properly.");  // 正常に値が読み取れませんでした
    }
    }
}

