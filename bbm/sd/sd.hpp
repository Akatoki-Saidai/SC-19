#ifndef SC19_PICO_SD_HPP_
#define SC19_PICO_SD_HPP_

#include<ctime>
#include<chrono>
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
    std::string filename_str = std::string("log_") + __DATE__[4] + __DATE__[5] + '_' + __TIME__[0] + __TIME__[1] + __TIME__[3] + __TIME__[4] + ".txt";
    const char* filename = filename_str.c_str();
public:
    SD();

    ~SD();

    void write(const std::string& write_str);
};

}


#endif  // SC19_PICO_SD_HPP_