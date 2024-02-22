#ifndef SC19_PICO_BBM_HPP_
#define SC19_PICO_BBM_HPP_

#include "sc.hpp"

#include "hcsr04/hcsr04.hpp"
#include "bme280/bme280.hpp"
#include "bno055/bno055_BBM.hpp"
#include "njl5513r/njl5513r.hpp"
#include "sd/sd.hpp"
#include "speaker/speaker.hpp"

void exit_16_callback(uint gpio, uint32_t emask)
{
    if (gpio != (volatile int)16) return;
    try {sc::print("\nIRQ Exit\n");}
    catch (...) {printf("\nIRQ Exit\n");}
    sleep_ms(100);
    exit((volatile int)0);
}

#endif  // SC19_PICO_BBM_HPP_
