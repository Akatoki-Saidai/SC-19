#include "bme280.hpp"
/*
これは Raspberry Pi で温湿度・気圧センサーBME280 を読み込むためのプログラムです．
通信方法はI2Cを使用しています．

このプログラムは，SPIを使用している以下のプログラムをI2C用に一部書き換えたものです．
    https://github.com/ms1963/bme280_spi/find/main
I2C用に書き換える際には，以下を参考にしました．
    https://qiita.com/jamjam/items/7f311463d09b17377225

ピンの接触不良が起きたときは
    湿度：0 %
くらいになります．
*/
namespace sc{
// Initialize BME280 sensor
BME280::BME280(const I2C& i2c) try :
    _i2c(i2c)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    try 
    {

        // this->i2c_no            = i2c_no;
        // this->sda_pin           = sda_pin;
        // this->scl_pin           = scl_pin;
        // this->_addr           = addr;
        // this->freq              = freq;
        // measurement_reg.mode    = mode;
        
        // switch (i2c_no) {
        //     case 0: i2c_hw = i2c0;
        //             break;
        //     case 1: i2c_hw = i2c1;
        //             break;
        //     default: return;
        // }
        // // initialize I2C access
        // i2c_init(i2c_hw, freq);

        //  // set all I2C GPIOs
        // gpio_set_function(scl_pin, GPIO_FUNC_I2C);
        // gpio_set_function(sda_pin, GPIO_FUNC_I2C);
        // gpio_pull_up(scl_pin);
        // gpio_pull_up(sda_pin);
        // See if SPI is working - interrograte the device for its I2C ID number, should be 0x60
        try
        {
            read_registers(0xD0, &chip_id, 1);
        }
        catch(const std::exception& e)
        {
            print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
            print(e.what());
            chip_id = 0x60;
        }
        
        // // 標高を計算する基準点をセット
        // pressure0    = 1013.25; //hPa
        // temperature0 = 20; //`C
        // altitude0    = 0; //m
    
        // read compensation params once
        read_compensation_parameters();

        try
        {
            measurement_reg.osrs_p = 0b011; // x4 Oversampling
            measurement_reg.osrs_t = 0b011; // x4 Oversampling
            write_register(0xF4, MODE::MODE_SLEEP); //SLEEP_MODE ensures configuration is saved
            // save configuration
            write_register(0xF2, 0x1); // Humidity oversampling register - going for x1
            write_register(0xF4, measurement_reg.get());// Set rest of oversampling modes and run mode to normal

            read();  // 最初の測定は誤差が大きいので，ここで測定しておく
        }
        catch(const std::exception& e)
        {
            print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
            print(e.what());
        }
    
    }
    catch(const std::exception& e)
    {
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
    }
}
catch (const std::exception& e)
{
    print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
}

// void BME280::set_origin(float _pressure0=1013.25, float _temperature0=20, float _altitude0=0){
//     #ifndef NODEBUG
//         std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
//     #endif
//     pressure0    = _pressure0;
//     temperature0 = _temperature0;
//     altitude0    = _altitude0;
// }

std::tuple<Pressure<Unit::Pa>,Humidity<Unit::percent>,Temperature<Unit::degC>> BME280::read() {
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (measurement_reg.mode == BME280::MODE::MODE_FORCED) {
        write_register(0xf4, measurement_reg.get());
        int count = 0;
        uint8_t buffer;
        do {
            read_registers(0xf3, &buffer, 1);
            sleep_ms(1);
            if (++count > 100) break;
        } while (buffer & 0x08); // loop until measurement completed
    }
    
    int32_t pressure[3], humidity[3], temperature[3];

    // 3回測定
    for (int i=0; i<3; ++i)
    {
        bme280_read_raw(&(humidity[i]), &(pressure[i]), &(temperature[i]));
        sleep_ms(1);
    }

    // 中央値を求める
    int32_t pressure_m = median(pressure[0], pressure[1], pressure[2]);
    int32_t humidity_m = median(humidity[0], humidity[1], humidity[2]);
    int32_t temperature_m = median(temperature[0], temperature[1], temperature[2]);

    pressure_m = compensate_pressure(pressure_m);
    humidity_m = compensate_humidity(humidity_m);
    temperature_m = compensate_temp(temperature_m);

    if (pressure_m < 900*100 || 1100*100 < pressure_m || humidity_m/1024 <= 0 || 100 <= humidity_m/1024 || temperature_m/100.0 < -20 || 50 < temperature_m/100.0)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "BME280 measurement value is abnormal"));  // BME280の測定値が異常です
    }

    Pressure<Unit::Pa>pressure_Pa(pressure_m);
    Humidity<Unit::percent>humidity_percent(humidity_m/1024.0);
    Temperature<Unit::degC>temperature_degC(temperature_m/100.0);
    // measurement.pressure = Pressure / 100.0;
    // measurement.humidity = Humidity / 1024.0;
    // measurement.temperature = Temperature / 100.0;

    // std::tuple<float,float,float>measurement(pressure,humidity,temperature);

    // // apply formula to retrieve altitude from air pressure
    // measurement.altitude_1 = altitude0 + ((temperature0 + 273.15F) / 0.0065F) * (1 - std::pow((measurement.pressure / pressure0), (1.0F / 5.257F)));
    // measurement.altitude_2 = altitude0 + ((measurement.temperature + 273.15F) / 0.0065F) * (std::pow((pressure0 / measurement.pressure), 1.0F / 5.257F) -1.0F);

    print("bme_read_data:%f,%f,%f\n", pressure_m, humidity_m/1024.0, temperature_m/100.0);

    return {pressure_Pa,humidity_percent,temperature_degC};

}

uint8_t BME280::get_chipID() {
    return chip_id;
}

// for the compensate_functions read the Bosch information on the BME280
int32_t BME280::compensate_temp(int32_t adc_T) {
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) * ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3))
            >> 14;

    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

uint32_t BME280::compensate_pressure(int32_t adc_P) {
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t) t_fine) >> 1) - (int32_t) 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) dig_P6);
    var2 = var2 + ((var1 * ((int32_t) dig_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t) dig_P4) << 16);
    var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t) dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t) dig_P1)) >> 15);
    if (var1 == 0)
        return 0;

    p = (((uint32_t) (((int32_t) 1048576) - adc_P) - (var2 >> 12))) * 3125;
    if (p < 0x80000000)
        p = (p << 1) / ((uint32_t) var1);
    else
        p = (p / (uint32_t) var1) * 2;

    var1 = (((int32_t) dig_P9) * ((int32_t) (((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t) (p >> 2)) * ((int32_t) dig_P8)) >> 13;
    p = (uint32_t) ((int32_t) p + ((var1 + var2 + dig_P7) >> 4));

    return p;
}

uint32_t BME280::compensate_humidity(int32_t adc_H) {
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t) 76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t) dig_H4) << 20) - (((int32_t) dig_H5) * v_x1_u32r)) +
                   ((int32_t) 16384)) >> 15) * (((((((v_x1_u32r * ((int32_t) dig_H6)) >> 10) * (((v_x1_u32r *
                                                                                                  ((int32_t) dig_H3))
            >> 11) + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152)) *
                                                 ((int32_t) dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    return (uint32_t) (v_x1_u32r >> 12);
}

void BME280::write_register(uint8_t reg, uint8_t data) {
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    uint8_t buf[2];
    //buf[0] = reg & 0x7f;  // remove read bit as this is a write
    buf[0] = reg;
    buf[1] = data;
    // i2c_write_blocking(i2c_hw, _addr, buf, 2, true);
    // sleep_ms(10);
    _i2c.write_memory(Binary(buf[1]),SlaveAddr(_addr),MemoryAddr(buf[0]));
}

void BME280::read_registers(uint8_t reg, uint8_t *buf, uint16_t len) {
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.
    // reg |= READ_BIT;  // 後で考える
    // i2c_write_blocking(i2c_hw, _addr, &reg, 1, true);
    // sleep_ms(10);
    // i2c_read_blocking(i2c_hw, _addr, buf, len, false);
    // sleep_ms(10);
    Binary b = _i2c.read_memory(size_t(len),SlaveAddr(_addr),MemoryAddr(reg));
    b.to_assign(buf);
}


/* This function reads the manufacturing assigned compensation parameters from the device */
void BME280::read_compensation_parameters() {
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif  
    
    try
    {
        read_registers(0x88, buffer, 26);

        dig_T1 = buffer[0] | (buffer[1] << 8);
        dig_T2 = buffer[2] | (buffer[3] << 8);
        dig_T3 = buffer[4] | (buffer[5] << 8);

        dig_P1 = buffer[6] | (buffer[7] << 8);
        dig_P2 = buffer[8] | (buffer[9] << 8);
        dig_P3 = buffer[10] | (buffer[11] << 8);
        dig_P4 = buffer[12] | (buffer[13] << 8);
        dig_P5 = buffer[14] | (buffer[15] << 8);
        dig_P6 = buffer[16] | (buffer[17] << 8);
        dig_P7 = buffer[18] | (buffer[19] << 8);
        dig_P8 = buffer[20] | (buffer[21] << 8);
        dig_P9 = buffer[22] | (buffer[23] << 8);

        dig_H1 = buffer[25];

        read_registers(0xE1, buffer, 8);

        dig_H2 = buffer[0] | (buffer[1] << 8);
        dig_H3 = (int8_t) buffer[2];
        dig_H4 = buffer[3] << 4 | (buffer[4] & 0xf);
        dig_H5 = (buffer[5] >> 4) | (buffer[6] << 4);
        dig_H6 = (int8_t) buffer[7];
    }
    catch(const std::exception& e)
    {
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
        print(e.what());
        dig_T1 = 28129;
        dig_T2 = 26436;
        dig_T3 = 50;
        dig_P1 = 38299;
        dig_P2 = -10600;
        dig_P3 = 3024;
        dig_P4 = 10670;
        dig_P5 = -305;
        dig_P6 = -7;
        dig_P7 = 9900;
        dig_P8 = -10230;
        dig_P9 = 4285;
        dig_H1 = 75;
        dig_H2 = 369;
        dig_H3 = 0;
        dig_H4 = 302;
        dig_H5 = 480;
        dig_H6 = -103;
    }
}

// this functions reads the raw data values from the sensor
void BME280::bme280_read_raw(int32_t *humidity, int32_t *pressure, int32_t *temperature) {
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    uint8_t readBuffer[8];
    read_registers(0xF7, readBuffer, 8);
    *pressure = ((uint32_t) readBuffer[0] << 12) | ((uint32_t) readBuffer[1] << 4) | (readBuffer[2] >> 4);
    *temperature = ((uint32_t) readBuffer[3] << 12) | ((uint32_t) readBuffer[4] << 4) | (readBuffer[5] >> 4);
    *humidity = (uint32_t) readBuffer[6] << 8 | readBuffer[7];
}
}