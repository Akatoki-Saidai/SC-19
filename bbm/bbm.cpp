#include "bbm.hpp"

namespace sc
{

extern float f_accelX, f_accelY, f_accelZ;

int main()
{
try
{
/***** setup *****/

    I2C bme_bno_i2c(SDA(6), SCL(7));
    // BME280 bme280(bme_bno_i2c);
    BNO055 bno055(bme_bno_i2c);
    

/***** loop *****/
    while (true)
    {
        // BME280::Measurement_t bme_data = bme280.measure();

        std::tuple bno_result = bno055.read();

        // print("bme temp:%f\n", bme_data.temperature);
        Acceleration<Unit::m_s2> accel_result = std::get<0>(bno_result);
        Acceleration<Unit::m_s2> gravity_result = std::get<1>(bno_result);
        MagneticFluxDensity<Unit::T> mag_result = std::get<2>(bno_result);
        AngularVelocity<Unit::rad_s> gyro_result = std::get<3>(bno_result);

        printf("accel");
        printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", accel_result[0],accel_result[1],accel_result[2]);
        printf("gravity");
        printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", gravity_result[0],gravity_result[1],gravity_result[2]);
        printf("mag");
        printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", mag_result[0],mag_result[1],mag_result[2]);        
        printf("gyro");
        printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", gyro_result[0],gyro_result[1],gyro_result[2]);

        sleep(1_s);
        

    }

    return 0;
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}

}

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("tareghieahgirefib\n");
    sc::main();
}