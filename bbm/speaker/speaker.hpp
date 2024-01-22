#ifndef SC19_PICO_SPEAKER_HPP_
#define SC19_PICO_SPEAKER_HPP_

#include <iostream>
#include "stdio.h"
#include "list"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "chrono"
#include "initializer_list"

// Class declaration
class SPEAKER {
public:
    SPEAKER();
    void speaker_init();    
    void play_starwars();
    void play_windows7();
    int  speaker_function();
};

#endif // SC19_PICO_SPEAKER_HPP_
