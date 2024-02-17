/**
モーターを動かす
 */

#include <iostream>
#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#define PIN_PWM1_1 11//モーター1の+
#define PIN_PWM1_2 10
#define PIN_PWM2_1 20//モーター2の+
#define PIN_PWM2_2 21

//引数にはduty比を入れる(duty比が大きいと速く進む)
//stoppage()はないので止めたいときは引数に0を入れる
void forward(double duty);
void back(double duty);
void right(double duty);
void left(double duty);

//PWMのチャンネル = ”slice”
//1
static pwm_config pwm1_1_slice_config;
static pwm_config pwm1_2_slice_config;
//2
static pwm_config pwm2_1_slice_config;
static pwm_config pwm2_2_slice_config;
//hardware/gpio.hに定義


void motor_init(void) {
    stdio_init_all();
    //1
    gpio_set_function(PIN_PWM1_1,GPIO_FUNC_PWM);
    gpio_set_function(PIN_PWM1_2,GPIO_FUNC_PWM);
    //GPIOはどのsliceか
    uint16_t pwm1_1_slice_num = pwm_gpio_to_slice_num(PIN_PWM1_1);
    uint16_t pwm1_2_slice_num = pwm_gpio_to_slice_num(PIN_PWM1_2);

    //2
    gpio_set_function(PIN_PWM2_1,GPIO_FUNC_PWM);
    gpio_set_function(PIN_PWM2_2,GPIO_FUNC_PWM);
    uint16_t pwm2_1_slice_num = pwm_gpio_to_slice_num(PIN_PWM2_1);
    uint16_t pwm2_2_slice_num = pwm_gpio_to_slice_num(PIN_PWM2_2);


    //デフォルト設定取得
    //1
    pwm1_1_slice_config = pwm_get_default_config();
    pwm1_2_slice_config = pwm_get_default_config();
    //2
    pwm2_1_slice_config = pwm_get_default_config();
    pwm2_2_slice_config = pwm_get_default_config();
    // 位相補正：なし
    // 分周：1分周
    // カウントモード：フリーランニング
    // 極性：通常   
    // ラップ値(一周期分のカウント値)：0xFFFF

    //normal mode config
    //pwm_init(pwm1_1_slice_num,&pwm1_1_slice_config,true);
    //pwm_init(pwm1_2_slice_num,&pwm1_slice_config,true);


    //1
    pwm_set_enabled(pwm1_1_slice_num, true);
    pwm_set_enabled(pwm1_2_slice_num, true);
    //2
    pwm_set_enabled(pwm2_1_slice_num, true);
    pwm_set_enabled(pwm2_2_slice_num, true);   
}    


void forward(double duty){
    //duty 50%
    pwm_set_gpio_level( PIN_PWM1_1, ( pwm1_1_slice_config.top * duty ) );
    pwm_set_gpio_level( PIN_PWM2_1, ( pwm2_1_slice_config.top * duty ) );
}

void back(double duty){
    pwm_set_gpio_level( PIN_PWM1_2, ( pwm1_2_slice_config.top * duty ) );
    pwm_set_gpio_level( PIN_PWM2_2, ( pwm2_2_slice_config.top * duty ) );
}

void right(double duty){
    pwm_set_gpio_level( PIN_PWM1_2, ( pwm1_1_slice_config.top * duty ) );
    pwm_set_gpio_level( PIN_PWM2_1, ( pwm2_1_slice_config.top * duty ) );
}

void left(double duty){
    pwm_set_gpio_level( PIN_PWM1_1, ( pwm1_1_slice_config.top * duty ) );
    pwm_set_gpio_level( PIN_PWM2_2, ( pwm2_1_slice_config.top * duty ) );
}


// int main() {
//     motor_init();
//     gpio_init(PICO_DEFAULT_LED_PIN);
//     gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
//     gpio_put(PICO_DEFAULT_LED_PIN, 1);

//     forward(1.0);
//     sleep_ms(2000);
//     forward(0);
//     sleep_ms(1000);
//     back(1.0);
//     sleep_ms(1000);
//     back(0);
//     //全部テスト

// }