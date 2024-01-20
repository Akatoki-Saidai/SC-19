#include "bbm.hpp"

namespace sc
{

int main()
{
/***** setup *****/

    uint8_t sda_pin = 5, scl_pin = 6;
    // BME280 bme(sda_pin, scl_pin);  // <== こんなかんじでセットアップしたい

/***** loop *****/
    while (true)
    {
        // bme_data = bme.read();  // <== こんなかんじで受信できるようにしたい
    }

    return 0;
}

}

int main()
{
    stdio_init_all();
    sc::main();
}