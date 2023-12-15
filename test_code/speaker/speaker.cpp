/**
スピーカーを鳴らす
 */

#include <iostream>
#include "stdio.h"
#include "vector"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "chrono"

#define PIN_Speaker_PWM 12
static pwm_config speaker_pwm_slice_config;
static uint8_t speaker_pwm_slice_num = pwm_gpio_to_slice_num(PIN_Speaker_PWM);
double speaker_pwm_clkdiv = 1.8;




void speaker_init(){

    //pwmの設定
    gpio_set_function(PIN_Speaker_PWM,GPIO_FUNC_PWM);

    // uint16_t speaker_pwm_slice_num = pwm_gpio_to_slice_num(PIN_Speaker_PWM);
    speaker_pwm_slice_config = pwm_get_default_config();
    // 位相補正：なし
    // 分周：1分周
    // カウントモード：フリーランニング
    // 極性：通常   
    // 1周期525.0us

    // double speaker_pwm_clkdiv = 1.8;
    pwm_config_set_clkdiv( &speaker_pwm_slice_config, speaker_pwm_clkdiv );

    pwm_set_enabled(speaker_pwm_slice_num, true);

}

void play_starwars(){

    
    //使用する音の周波数の宣言(低いラ～高いド)
    const double sound_A4 = 440; //ラ
    const double sound_B4 = 493.883;
    const double sound_C5 = 523.251;
    const double sound_D5 = 587.330;
    const double sound_E5 = 659.255;
    const double sound_F5 = 698.456;
    const double sound_G5 = 783.991;
    const double sound_A5 = 880;
    const double sound_B5 = 987.767;
    const double sound_C6 = 1046.502;


    const double starwars_bps = 7.695;
    const double starwars_mspb = 1 / starwars_bps;
    // bps = 153.9 原曲153.9bpm / 60秒 = 2.565bps * 3連符 = 7.695
    // mspb = 0.12995... # 7.695bpsの逆数 = 0.12995...s　4部音符ひとつ分の音の長さ(音の間隔)

    // メロディーを配列で作成 0は無音(休符)
    const std::vector<double> starwars_melody = {sound_C5, sound_C5, sound_G5, sound_G5, sound_F5, sound_E5, sound_D5, sound_C6, sound_C6, sound_G5, sound_G5, sound_F5, sound_E5, sound_D5, sound_C6, sound_C6, sound_G5, sound_G5, sound_F5, sound_E5, sound_F5, sound_D5 };

    //ドーソーファミレド(高)ーソーファミレド(高)ーソーファミファレー


    for(uint8_t melody_order = 0; melody_order <= starwars_melody.size(); melody_order++){

        if(starwars_melody[melody_order] == 0){
            
        }

        else{
            static const uint32_t Raspberry_pi_clock = 125000000;
            static double speaker_duty = 0.50;

            // メモ
            // duty比は周期、周波数に影響しない
            // 一回のカウントクロック(ラップ) = 62500
            // ラップ値 = 周期が始まってから再び0になるクロック数
            // 分周比n = nのクロック周期を1のクロック周期とみなし、出力周波数が1/nする
            // picoのクロックは125000000Hz
            // 1周期の秒数 = ((ラップ + 1) * 分周比) / 125000000
            // 出力周波数 = 125000000 / ((ラップ + 1) * 分周比)
            // ラップ  = (125000000 / (f * 分周比)) - 1

            uint16_t speaker_pwm_wrap = (Raspberry_pi_clock / (starwars_melody[melody_order] * speaker_pwm_clkdiv)) - 1;
            pwm_config_set_wrap( &speaker_pwm_slice_config, speaker_pwm_wrap );

            pwm_init( speaker_pwm_slice_num, &speaker_pwm_slice_config, true );

            pwm_set_gpio_level( PIN_Speaker_PWM, ( speaker_pwm_wrap * speaker_duty ) );
            
            }

        // 音の継続
        double sound_start_clock = clock();
        while(clock() - sound_start_clock >= starwars_mspb){

        }

    }
}


int main(){
    speaker_init();
    play_starwars();
}

