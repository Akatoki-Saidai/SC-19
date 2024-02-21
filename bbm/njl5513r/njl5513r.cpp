/*
バイオモニタリングセンサ用プログラム
(機体放出検知，LED)
 */

// #include <iostream>
// #include "stdio.h"
// #include "pico/stdlib.h"
// #include "hardware/gpio.h"
// #include "hardware/adc.h"//CMakeList.txtのtarget_link_librariesに追加

#include "njl5513r.hpp"

#define ADC_Lux_Sensor 26 // 照度用GPIO
#define GPIO_RED_LED 27 // 赤LED用GPIO
#define GPIO_GREEN_LED 28 // 緑LED用GPIO

void bio_adc_init(){
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    adc_init();
    adc_gpio_init(ADC_Lux_Sensor); //ADC0(GPIO26)の端子設定
    adc_select_input(0); //ADC0を設定

}

void bio_LED_init(){
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    gpio_init(GPIO_RED_LED);
    gpio_init(GPIO_GREEN_LED);
    gpio_set_dir(GPIO_RED_LED, GPIO_OUT);
    gpio_set_dir(GPIO_GREEN_LED, GPIO_OUT);
}

// int main() {
//     stdio_init_all();
//     gpio_init(PICO_DEFAULT_LED_PIN);
//     gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
//     bio_adc_init();

//     while(true){

//         //照度
//         static uint8_t carrier_open = 0;
//         // 指を付けたら310くらい、通常時で420くらい(全学2-103)
//         int32_t Transition_lux = adc_read();

//         std::cout << "lux: " << Transition_lux << std::endl;

//         // 照度値が閾値を超えたらPhaseを1にする
//         if (Transition_lux >= 340 && carrier_open != 1) {
//             carrier_open = 1;
//             std::cout << "Carrier was open" << std::endl;
//         }

//         if (carrier_open == 0) {
//             gpio_put( PICO_DEFAULT_LED_PIN, 0);
//             sleep_ms(1000); 
//             gpio_put( PICO_DEFAULT_LED_PIN, 0);
//         }
//         else{
//             gpio_put( PICO_DEFAULT_LED_PIN, 1);
//             sleep_ms(1000); 
//             gpio_put( PICO_DEFAULT_LED_PIN, 0);
//         }


//         //LED
//         gpio_put(GPIO_RED_LED, 1);
//         sleep_ms(1000);
//         gpio_put(GPIO_RED_LED, 0);
//         gpio_put(GPIO_GREEN_LED, 1);
//         sleep_ms(1000);
//         gpio_put(GPIO_GREEN_LED, 0);

//         std::cout << "LED lighted" << std::endl;
//     }

// }
