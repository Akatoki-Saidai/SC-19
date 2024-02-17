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
#include "sc.hpp"

namespace sc 
{

// Class declaration
class Speaker {
    const Pin& _pin;
    const uint8_t _speaker_pwm_slice_num = pwm_gpio_to_slice_num(_pin.gpio());
    pwm_config _speaker_pwm_slice_config;
    static constexpr double _speaker_pwm_clkdiv = 10;

public:
    Speaker(const Pin& pin);
    void play_starwars();
    void play_windows7();
};

}

#endif // SC19_PICO_SPEAKER_HPP_
