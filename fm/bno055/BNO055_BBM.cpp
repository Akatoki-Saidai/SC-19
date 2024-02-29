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

// //磁気
// // uint8_t mag[6];   // magnetometer registers
// int16_t magX, magY, magZ;
// float f_magX, f_magY, f_magZ;
// uint8_t mag_val = 0x0E;

// //ジャイロセンサ
// // uint8_t gyro[6];
// int16_t gyroX, gyroY, gyroZ;
// float f_gyroX, f_gyroY, f_gyroZ;
// uint8_t gyro_val = 0x14;

// //重力加速度
// // uint8_t grv[6];
// int16_t grvX, grvY, grvZ;
// float f_grvX, f_grvY, f_grvZ;
// uint8_t grv_val = 0x2E; //GRV_DATA_X_LSB 0x2E

// //線形加速度
// // uint8_t accel[6];
// int16_t accelX, accelY, accelZ;
// float f_accelX, f_accelY, f_accelZ;
// uint8_t accel_val = 0x28; //LIA_DATA_X_LSB 0x28  

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

    int16_t magX[3], magY[3], magZ[3];  // 磁気
    int16_t gyroX[3], gyroY[3], gyroZ[3];  // ジャイロ
    int16_t grvX[3], grvY[3], grvZ[3];  // 重力加速度
    int16_t accelX[3], accelY[3], accelZ[3];  // 線形加速度
    
    constexpr uint8_t accel_val = 0x28; // 線形加速度のメモリアドレス

    //線形加速度を読み取る
    for (int i=0; i<3; ++i)
    {
        Binary accel = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(accel_val));

        accelX[i] = ((accel[1]<<8) | accel[0]);
        accelY[i] = ((accel[3]<<8) | accel[2]);
        accelZ[i] = ((accel[5]<<8) | accel[4]);

        sleep_ms(1);
    }

    double d_accelX = median(accelX[0], accelX[1], accelX[2]) / 100.00;
    double d_accelY = median(accelY[0], accelY[1], accelY[2]) / 100.00;
    double d_accelZ = median(accelZ[0], accelZ[1], accelZ[2]) / 100.00;

    if (std::abs(d_accelX) > 30 || std::abs(d_accelY) > 30 || std::abs(d_accelZ) > 30)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "BNO055 measurement value is abnormal. accel:%f, %f, %f", d_accelX, d_accelY, d_accelZ));  // BNO055の測定値が異常です
    }

    Acceleration<Unit::m_s2>accel_vector{dimension::m_s2(d_accelX),dimension::m_s2(d_accelY),dimension::m_s2(d_accelZ)};

    constexpr uint8_t grv_val = 0x2E; // 重力加速度のメモリアドレス

    //重力加速度
    for (int i=0; i<3; ++i)
    {
        Binary grv = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(grv_val));

        grvX[i] = ((grv[1]<<8) | grv[0]);
        grvY[i] = ((grv[3]<<8) | grv[2]);
        grvZ[i] = ((grv[5]<<8) | grv[4]);

        sleep_ms(1);
    }

    double d_grvX = median(grvX[0], grvX[1], grvX[2]) / 100.00;
    double d_grvY = median(grvY[0], grvY[1], grvY[2]) / 100.00;
    double d_grvZ = median(grvZ[0], grvZ[1], grvZ[2]) / 100.00;

    if (std::abs(9.8 - std::sqrt(d_grvX*d_grvX + d_grvY*d_grvY + d_grvZ*d_grvZ)) > 0.1)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "BNO055 measurement value is abnormal. grv:%f, %f, %f", d_grvX, d_grvY, d_grvZ));  // BNO055の測定値が異常です
    }

    Acceleration<Unit::m_s2>grav_vector{dimension::m_s2(d_grvX),dimension::m_s2(d_grvY),dimension::m_s2(d_grvZ)};

    constexpr uint8_t mag_val = 0x0E;  // 磁気のメモリアドレス

    //地磁気
    for (int i=0; i<3; ++i)
    {
        Binary mag = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(mag_val));

        magX[i] = ((mag[1] << 8) | mag[0]);
        magY[i] = ((mag[3] << 8) | mag[2]);
        magZ[i] = ((mag[5] << 8) | mag[4]);

        sleep_ms(1);
    }

    double d_magX = milli * median(magX[0], magX[1], magX[2]) / 16.00;
    double d_magY = milli * median(magY[0], magY[1], magY[2]) / 16.00;
    double d_magZ = milli * median(magZ[0], magZ[1], magZ[2]) / 16.00;

    double all_mag =std::sqrt(d_magX*d_magX + d_magY*d_magY + d_magZ*d_magZ);    
    if (all_mag < 35*milli || 70*milli < all_mag)  // 日本は47mT～50mTくらい
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "BNO055 measurement value is abnormal. mag:%f, %f, %f", d_magX, d_magY, d_magZ));  // BNO055の測定値が異常です
    }
    
    MagneticFluxDensity<Unit::T>Mag_vector{dimension::T(d_magX),dimension::T(d_magY),dimension::T(d_magZ)};
    
    constexpr uint8_t gyro_val = 0x14;  // ジャイロのメモリアドレス

    //ジャイロ
    for (int i=0; i<3; ++i)
    {
        Binary gyro = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(gyro_val));

        gyroX[i] = ((gyro[1] << 8) | gyro[0]);
        gyroY[i] = ((gyro[3] << 8) | gyro[2]);
        gyroZ[i] = ((gyro[5] << 8) | gyro[4]);

        sleep_ms(1);
    }

    double d_gyroX = median(gyroX[0], gyroX[1], gyroX[2]) / 900.00;
    double d_gyroY = median(gyroY[0], gyroY[1], gyroY[2]) / 900.00;
    double d_gyroZ = median(gyroZ[0], gyroZ[1], gyroZ[2]) / 900.00;

    if (d_gyroX > 20 || d_gyroY > 20 || d_gyroZ > 20)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "BNO055 measurement value is abnormal. gyro:%f, %f, %f", d_gyroX, d_gyroY, d_gyroZ));  // BNO055の測定値が異常です
    }

    AngularVelocity<Unit::rad_s>gyro_vector{dimension::rad_s(d_gyroX),dimension::rad_s(d_gyroY),dimension::rad_s(d_gyroZ)};

    // 測定値がおかしくなったら再び初期化
    if (d_accelX==0 && d_accelY==0 && d_accelZ==0 && d_grvX==0 && d_grvY==0 && d_grvZ==0 && d_magX==0 && d_magY==0 && d_magZ==0 && d_gyroX==0 && d_gyroY==0 && d_gyroZ==0)
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