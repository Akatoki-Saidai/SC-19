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
    print("gseivsbvisea\n");
    // BME280 bme280(bme_bno_i2c);
    BNO055 bno055(bme_bno_i2c);
    

/***** loop *****/
    while (true)
    {
        printf("aiueo\n");
        // BME280::Measurement_t bme_data = bme280.measure();
        bno055.read();

        // print("bme temp:%f\n", bme_data.temperature);
        print("bno accel:%f, %f, %f\n", f_accelX, f_accelY, f_accelZ);
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