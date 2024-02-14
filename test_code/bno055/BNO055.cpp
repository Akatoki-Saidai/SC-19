#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1

static int addr = 0x28;

// Initialise Accelerometer Function
void accel_init(void){
    // 接続が正常か確かめる
    sleep_ms(1000);
    uint8_t reg = 0x00;
    uint8_t chipID[1];
    i2c_write_blocking(I2C_PORT, addr, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, addr, chipID, 1, false);

    if(chipID[0] != 0xA0){
        while(1){
            printf("Chip ID Not Correct - Check Connection!");
            sleep_ms(5000);
        }
    }

    // Use internal oscillator
    uint8_t data[2];
    data[0] = 0x3F;
    data[1] = 0x40;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);

    // Reset all interrupt status bits
    data[0] = 0x3F;
    data[1] = 0x01;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);

    // Configure Power Mode
    data[0] = 0x3E;
    data[1] = 0x00;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    sleep_ms(50);

    // Defaul Axis Configuration
    data[0] = 0x41;
    data[1] = 0x24;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);

    // Default Axis Signs
    data[0] = 0x42;
    data[1] = 0x00;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);

    // Set units to m/s^2
    data[0] = 0x3B;
    data[1] = 0b0001000;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    sleep_ms(30);

    // //モードをAMG(Accel Mag Gyro)に設定
    // data[0] = 0x3D;
    // data[1] = 0b0111;
    // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    // sleep_ms(100);

    //モードをNDOFに設定
    //メモ:NDOFは加速度を分けれるFusion modeに対応している
    //また、Fusion Dataは地球の補正が入ったAbsolute orientationになっている
    //解釈違いがあるかもなので詳しくはData Sheet p21~
    data[0] = 0x3D;
    data[1] = 0b1100;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    sleep_ms(100);

    //メモ:SC-17のコードでもdata[1] = 0x0C = (int)12となっている??
}

int main(void){
    stdio_init_all(); // Initialise STD I/O for printing over serial

    // Configure the I2C Communication
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(6, GPIO_FUNC_I2C);
    gpio_set_function(7, GPIO_FUNC_I2C);
    gpio_pull_up(6);
    gpio_pull_up(7);

    // Call accelerometer initialisation function
    accel_init();

    //全加速度
    // uint8_t accel[6]; // Store data from the 6 acceleration registers
    // int16_t accelX, accelY, accelZ; // Combined 3 axis data
    // float f_accelX, f_accelY, f_accelZ; // Float type of acceleration data
    // uint8_t accel_val = 0x08; // ACC_DATA_XLSB 0x08

    //磁気
    uint8_t mag[6];   // magnetometer registers
    int16_t magX, magY, magZ;
    float f_magX, f_magY, f_magZ;
    uint8_t mag_val = 0x0E; //MAG_DATA_XLSB 0x0E

    //ジャイロセンサ
    uint8_t gyro[6];
    int16_t gyroX, gyroY, gyroZ;
    float f_gyroX, f_gyroY, f_gyroZ;
    uint8_t gyro_val = 0x14; //GYR_DATA_X_LSB 0x14

    //重力加速度
    uint8_t grv[6];
    int16_t grvX, grvY, grvZ;
    float f_grvX, f_grvY, f_grvZ;
    uint8_t grv_val = 0x2E; //GRV_DATA_X_LSB 0x2E

    //加速度
    uint8_t accel[6];
    int16_t accelX, accelY, accelZ;
    float f_accelX, f_accelY, f_accelZ;
    uint8_t accel_val = 0x28; //LIA_DATA_X_LSB 0x28  


    // Infinite Loop
    while(1){
        //read acceleration data
        i2c_write_blocking(I2C_PORT, addr, &accel_val, 1, true);
        i2c_read_blocking(I2C_PORT, addr, accel, 6, false);

        accelX = ((accel[1]<<8) | accel[0]);
        accelY = ((accel[3]<<8) | accel[2]);
        accelZ = ((accel[5]<<8) | accel[4]);

        f_accelX = accelX / 100.00;
        f_accelY = accelY / 100.00;
        f_accelZ = accelZ / 100.00;


        //read gravity data
        i2c_write_blocking(I2C_PORT, addr, &grv_val, 1, true);
        i2c_read_blocking(I2C_PORT, addr, grv, 6, false);

        accelX = ((grv[1]<<8) | grv[0]);
        accelY = ((grv[3]<<8) | grv[2]);
        accelZ = ((grv[5]<<8) | grv[4]);

        f_grvX = grvX / 100.00;
        f_grvY = grvY / 100.00;
        f_grvZ = grvZ / 100.00;

        // Read magnetometer data
        i2c_write_blocking(I2C_PORT, addr, &mag_val, 1, true);
        i2c_read_blocking(I2C_PORT, addr, mag, 6, false);
        magX = ((mag[1] << 8) | mag[0]);
        magY = ((mag[3] << 8) | mag[2]);
        magZ = ((mag[5] << 8) | mag[4]);

        f_magX = magX / 16.00;
        f_magY = magY / 16.00;
        f_magZ = magZ / 16.00;

        // Read gyro data
        i2c_write_blocking(I2C_PORT, addr, &gyro_val, 1, true);
        i2c_read_blocking(I2C_PORT, addr, gyro, 6, false);
        gyroX = ((gyro[1] << 8) | gyro[0]);
        gyroY = ((gyro[3] << 8) | gyro[2]);
        gyroZ = ((gyro[5] << 8) | gyro[4]);

        f_gyroX = gyroX / 900.00;
        f_gyroY = gyroY / 900.00;
        f_gyroZ = gyroZ / 900.00;

        // Print to serial monitor
        printf("accel");
        printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_accelX, f_accelY, f_accelZ);
        printf("gravity");
        printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_grvX, f_grvY, f_grvZ);
        printf("mag  ");
        printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_magX, f_magY, f_magZ);
        printf("gyro ");
        printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_gyroX, f_gyroY, f_gyroZ);
        printf("\n\n");
        sleep_ms(500);
        
    }
}