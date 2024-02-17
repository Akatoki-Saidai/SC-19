#ifndef SC19_PICO_SD_HPP_
#define SC19_PICO_SD_HPP_

#include<ctime>
#include <stdio.h>
#include<string>
//
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
//
#include "hw_config.h"

#include "sc.hpp"

namespace sc 
{

// class SD;

class SD 
{
    sd_card_t *pSD;
    FRESULT fr;
    const char* filename = []{
            char time_str[17];
            std::time_t t = std::time(nullptr);
            std::strftime(time_str, sizeof(time_str), "%Y%m%d_%H%M%S", std::localtime(&t));
            return (std::string("log_") + time_str).c_str();
        }();
public:
    SD(MISO miso, MOSI mosi, SCK sck, Pin ss, Pin card_detect, dimension::Hz freq = 12'500'000_hz);

    ~SD();

    void write(std::string& write_str);
};

}


#endif  // SC19_PICO_SD_HPP_