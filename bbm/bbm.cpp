#include "bbm.hpp"

namespace sc
{

int main()
{
/***** setup *****/

    I2C i2c(SDA(0), SCL(1));
    BME280 bme(i2c);  // <== こんなかんじでセットアップしたい

/***** loop *****/
    while (true)
    {
        bme_data = bme.read();  // <== こんなかんじで受信できるようにしたい
    }

    return 0;
}

}

int main()
{
    stdio_init_all();
    sc::main();
}