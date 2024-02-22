#include "BNO055_BBM.hpp" //クラス定義

//BNO055::BNO055()とint BNO55::get_BNO055()内で使われている関数を先に定義しておく？

//一応ifndefにしておいた
// #ifndef I2C_PORT i2c1
// #define I2C_PORT i2c1
// #endif

namespace sc 
{

// void accel_init(void);
static int addr = 0x28;

// //全加速度
// // uint8_t accel[6]; // Store data from the 6 acceleration registers
// int16_t accelX, accelY, accelZ; // Combined 3 axis data
// float f_accelX, f_accelY, f_accelZ; // Float type of acceleration data
// uint8_t accel_val = 0x08; // Start register address

//磁気
// uint8_t mag[6];   // magnetometer registers
int16_t magX, magY, magZ;
float f_magX, f_magY, f_magZ;
uint8_t mag_val = 0x0E;

//ジャイロセンサ
// uint8_t gyro[6];
int16_t gyroX, gyroY, gyroZ;
float f_gyroX, f_gyroY, f_gyroZ;
uint8_t gyro_val = 0x14;

//重力加速度
// uint8_t grv[6];
int16_t grvX, grvY, grvZ;
float f_grvX, f_grvY, f_grvZ;
uint8_t grv_val = 0x2E; //GRV_DATA_X_LSB 0x2E

//線形加速度
// uint8_t accel[6];
int16_t accelX, accelY, accelZ;
float f_accelX, f_accelY, f_accelZ;
uint8_t accel_val = 0x28; //LIA_DATA_X_LSB 0x28  

//ここまで


//以下class BNO055で定義した関数とかの中身

BNO055::BNO055(const I2C& i2c) try :
    _i2c(i2c)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif

    // // Configure the I2C Communication
    // // i2c_init(I2C_PORT, 400 * 1000);
    // gpio_set_function(6, GPIO_FUNC_I2C);
    // gpio_set_function(7, GPIO_FUNC_I2C);
    // gpio_pull_up(6);
    // gpio_pull_up(7);

    // Call accelerometer initialisation function
    accel_init();

}
catch(const std::exception& e)
{
    print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
    print(e.what());
}

std::tuple<Acceleration<Unit::m_s2>,Acceleration<Unit::m_s2>,MagneticFluxDensity<Unit::T>,AngularVelocity<Unit::rad_s>> BNO055::read(){
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    //線形加速度を読み取る
    // i2c_write_blocking(I2C_PORT, addr, &accel_val, 1, true);
    // i2c_read_blocking(I2C_PORT, addr, accel, 6, false);
    Binary accel = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(accel_val));

    accelX = ((accel[1]<<8) | accel[0]);
    accelY = ((accel[3]<<8) | accel[2]);
    accelZ = ((accel[5]<<8) | accel[4]);

    f_accelX = accelX / 100.00;
    f_accelY = accelY / 100.00;
    f_accelZ = accelZ / 100.00;

    Acceleration<Unit::m_s2>accel_vector{dimension::m_s2(f_accelX),dimension::m_s2(f_accelY),dimension::m_s2(f_accelZ)};

    //重力加速度
    // i2c_write_blocking(I2C_PORT, addr, &grv_val, 1, true);
    // i2c_read_blocking(I2C_PORT, addr, grv, 6, false);
    Binary grv = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(grv_val));

    grvX = ((grv[1]<<8) | grv[0]);
    grvY = ((grv[3]<<8) | grv[2]);
    grvZ = ((grv[5]<<8) | grv[4]);

    f_grvX = grvX / 100.00;
    f_grvY = grvY / 100.00;
    f_grvZ = grvZ / 100.00;

    Acceleration<Unit::m_s2>grav_vector{dimension::m_s2(f_grvX),dimension::m_s2(f_grvY),dimension::m_s2(f_grvZ)};

    //地磁気
    // i2c_write_blocking(I2C_PORT, addr, &mag_val, 1, true);
    // i2c_read_blocking(I2C_PORT, addr, mag, 6, false);
    Binary mag = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(mag_val));

    magX = ((mag[1] << 8) | mag[0]);
    magY = ((mag[3] << 8) | mag[2]);
    magZ = ((mag[5] << 8) | mag[4]);

    f_magX = magX / 16.00;
    f_magY = magY / 16.00;
    f_magZ = magZ / 16.00;
    
    MagneticFluxDensity<Unit::T>Mag_vector{dimension::T(f_magX),dimension::T(f_magY),dimension::T(f_magZ)};
    
    //ジャイロ
    // i2c_write_blocking(I2C_PORT, addr, &gyro_val, 1, true);
    // i2c_read_blocking(I2C_PORT, addr, gyro, 6, false);
    Binary gyro = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(gyro_val));

    gyroX = ((gyro[1] << 8) | gyro[0]);
    gyroY = ((gyro[3] << 8) | gyro[2]);
    gyroZ = ((gyro[5] << 8) | gyro[4]);

    f_gyroX = gyroX / 900.00;
    f_gyroY = gyroY / 900.00;
    f_gyroZ = gyroZ / 900.00;

    AngularVelocity<Unit::rad_s>gyro_vector{dimension::rad_s(f_gyroX),dimension::rad_s(f_gyroY),dimension::rad_s(f_gyroZ)};

    // 測定値がおかしくなったら再び初期化
    if (accelX==0 && accelY==0 && accelZ==0 && grvX==0 && grvY==0 && grvZ==0 && magX==0 && magY==0 && magZ==0 && gyroX==0 && gyroY==0 && gyroZ==0)
    {
        print("!!reinitialize BNO!!\n");  // BNOを再び初期化します
        accel_init();
        sleep(100_ms);
        throw std::runtime_error(f_err(__FILE__, __LINE__, "BNO055 measurement value is abnormal"));  // BNO055の測定値が異常です
    }

    return {accel_vector,grav_vector,Mag_vector,gyro_vector};
}


//最初に定義した関数とかの中身

// Initialise Accelerometer Function
void BNO055::accel_init(void){
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    // Check to see if connection is correct
    sleep_ms(1000); // Add a short delay to help BNO005 boot up
    uint8_t reg = 0x00;
    uint8_t chipID[1];
    // i2c_write_blocking(i2c1, addr, &reg, 1, true);
    // i2c_read_blocking(i2c1, addr, chipID, 1, false);
    try
    {
        chipID[0] = _i2c.read_memory(size_t(1), SlaveAddr(addr), MemoryAddr(reg)).at(0);
        if(chipID[0] != 0xA0)
        {
            printf("Chip ID Not Correct - Check Connection!");
        }
    }
    catch(const std::exception& e)
    {
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
        print(e.what());
        chipID[0] = 0xA0;
    }
    
    try
    {
        // Use internal oscillator
        uint8_t data[2];
        data[0] = 0x3F;
        data[1] = 0x40;
        // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
        _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));

        // Reset all interrupt status bits
        data[0] = 0x3F;
        data[1] = 0x01;
        // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
        _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));

        // Configure Power Mode
        data[0] = 0x3E;
        data[1] = 0x00;
        // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
        _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));
        sleep_ms(50);

        // Defaul Axis Configuration
        data[0] = 0x41;
        data[1] = 0x24;
        // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
        _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));

        // Default Axis Signs
        data[0] = 0x42;
        data[1] = 0x00;
        // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
        _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));

        // Set units to m/s^2
        data[0] = 0x3B;
        data[1] = 0b0001000;
        // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
        _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));
        sleep_ms(30);

        // Set operation to AMG(Accel Mag Gyro)
        data[0] = 0x3D;
        data[1] = 0b1100;
        // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
        _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));
        sleep_ms(100);    
    }
    catch(const std::exception& e)
    {
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
        print(e.what());
    }
}

}