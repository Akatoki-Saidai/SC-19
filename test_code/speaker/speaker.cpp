/**
スピーカーを鳴らす
 */

#include <iostream>
#include "stdio.h"
#include "list"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "chrono"
#include "initializer_list"

#define PIN_Speaker_PWM 22
static pwm_config speaker_pwm_slice_config;
static uint8_t speaker_pwm_slice_num = pwm_gpio_to_slice_num(PIN_Speaker_PWM);
double speaker_pwm_clkdiv = 5.5;




void speaker_init(){

    //pwmの設定
    gpio_set_function(PIN_Speaker_PWM,GPIO_FUNC_PWM);

    // uint8_t speaker_pwm_slice_num = pwm_gpio_to_slice_num(PIN_Speaker_PWM);
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

    
    //使用する音の周波数の宣言(低いラ～高いド) 0は無音(休符)
    const double sound_rest = 0;
    const double sound_G4 = 391.995;
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


    const double starwars_bps = 163.9 / 60;
    const double starwars_spb = 1 / starwars_bps;
    // bpm = 153.9 原曲153.9bpm / 60秒 = 2.565bps
    // spb = 0.38986  2.565bpsの逆数: 4部音符ひとつ分の音の長さ(音の間隔)

    // メロディーを配列で作成
    const std::initializer_list<double> starwars_melody{ sound_C5, sound_C5, sound_G5, sound_G5, sound_F5, sound_E5, sound_D5, sound_C6, sound_C6, sound_G5, sound_G5, sound_F5, sound_E5, sound_D5, sound_C6, sound_C6, sound_G5, sound_G5, sound_F5, sound_E5, sound_F5, sound_D5, sound_D5 };
    // ソ(低) ソ(低) ソ(低)ドーソーファミレド(高)ーソーファミレド(高)ーソーファミファレー
    auto starwars_melody_now_itr =starwars_melody.begin();


    for(uint8_t starwars_melody_order = 0; starwars_melody_order <= starwars_melody.size(); starwars_melody_order++){

        double sound_start_clock = clock();
        
        double starwars_melody_now = *starwars_melody_now_itr;

        if(starwars_melody_now == sound_rest){
            pwm_set_gpio_level( PIN_Speaker_PWM, ( sound_rest ) );
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

            uint16_t speaker_pwm_wrap = (Raspberry_pi_clock / (starwars_melody_now * speaker_pwm_clkdiv)) - 1;

            pwm_config_set_wrap( &speaker_pwm_slice_config, speaker_pwm_wrap );
            pwm_init( speaker_pwm_slice_num, &speaker_pwm_slice_config, true );

            pwm_set_gpio_level( PIN_Speaker_PWM, ( speaker_pwm_wrap * speaker_duty ) );
        }
        
        
        // 次の音
        starwars_melody_now_itr = ++starwars_melody_now_itr;
        
        // 音の継続
        while( (clock() - sound_start_clock) / CLOCKS_PER_SEC < starwars_spb){
        }
        
        // std::cout << (clock() - sound_start_clock) / CLOCKS_PER_SEC << std::endl;

    }

    // 演奏終了
    pwm_set_gpio_level( PIN_Speaker_PWM, ( sound_rest ) );

}

void play_windows7(){
    // 使用する音
    const double sound_rest = 0;
    const double sound_B4 = 493.883;
    const double sound_E5 = 659.255;
    const double sound_Fs5 = 761.672;
    const double sound_B5 = 987.767;

    const double windows7_bps = 120.0 / 60;
    const double windows7_spb = 1 / windows7_bps;

    const std::initializer_list<double> windows7_melody{sound_B4, sound_E5, sound_Fs5, sound_B5};
    auto windows7_melody_now_itr =windows7_melody.begin();


    for(uint8_t windows7_melody_order = 0; windows7_melody_order < windows7_melody.size(); windows7_melody_order++){

        double sound_start_clock = clock();
        
        double windows7_melody_now = *windows7_melody_now_itr;

        static const uint32_t Raspberry_pi_clock = 125000000;
        static double speaker_duty = 0.50;

        uint16_t speaker_pwm_wrap = (Raspberry_pi_clock / (windows7_melody_now * speaker_pwm_clkdiv)) - 1;

        pwm_config_set_wrap( &speaker_pwm_slice_config, speaker_pwm_wrap );
        pwm_init( speaker_pwm_slice_num, &speaker_pwm_slice_config, true );

        pwm_set_gpio_level( PIN_Speaker_PWM, ( speaker_pwm_wrap * speaker_duty ) );
        
        // 次の音
        windows7_melody_now_itr = ++windows7_melody_now_itr;
        
        // 音の継続
        if (windows7_melody_order == 0 || windows7_melody_order == 2){   
            while( (clock() - sound_start_clock) / CLOCKS_PER_SEC <= windows7_spb / 2){
            }
        }
        else{
            while( (clock() - sound_start_clock) / CLOCKS_PER_SEC <= windows7_spb){
            }
        }

    // 演奏終了
    pwm_set_gpio_level( PIN_Speaker_PWM, ( sound_rest ) );
    }

}


void play_hogwarts(){
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    
    //使用する音の周波数の宣言
    const double sound_rest = 0;
    // const double sound_A4 = 440;  // ラ
    const double sound_B4 = 493.883;  // シ
    const double sound_Ds5 = 622.254;  // レ#
    const double sound_E5 = 659.255;  // ミ
    const double sound_F5 = 698.456;  // ファ
    const double sound_Fs5 = 739.989;  // ファ#
    const double sound_G5 = 783.991;  // ソ
    const double sound_A5 = 880.0;  // ラ
    const double sound_B5 = 987.767;  // シ


    const double hogwarts_bps = 146.0 / 60;
    const double hogwarts_spb = 1 / hogwarts_bps;

    // メロディーを配列で作成
    const std::initializer_list<double> hogwarts_melody{ sound_B4, sound_E5, sound_E5, sound_G5, sound_Fs5, sound_Fs5, sound_E5, sound_E5, sound_B5, sound_A5, sound_A5, sound_Fs5, sound_Fs5, sound_rest, sound_E5, sound_E5, sound_G5, sound_Fs5, sound_Ds5, sound_Ds5, sound_F5, sound_B4 };

    auto hogwarts_melody_now_itr =hogwarts_melody.begin();


    for(uint8_t hogwarts_melody_order = 0; hogwarts_melody_order < hogwarts_melody.size(); hogwarts_melody_order++){

        double sound_start_clock = clock();
        
        double hogwarts_melody_now = *hogwarts_melody_now_itr;

        if(hogwarts_melody_now == sound_rest){
            pwm_set_gpio_level( PIN_Speaker_PWM, ( sound_rest ) );
        }

        else{
            static const uint32_t Raspberry_pi_clock = 125000000;
            static double speaker_duty = 0.50;

            uint16_t speaker_pwm_wrap = (Raspberry_pi_clock / (hogwarts_melody_now * speaker_pwm_clkdiv)) - 1;

            pwm_config_set_wrap( &speaker_pwm_slice_config, speaker_pwm_wrap );
            pwm_init( speaker_pwm_slice_num, &speaker_pwm_slice_config, true );

            pwm_set_gpio_level( PIN_Speaker_PWM, ( speaker_pwm_wrap * speaker_duty ) );
        }
        
        
        // 次の音
        ++hogwarts_melody_now_itr;
        
        // 音の継続
        if (hogwarts_melody_order == 16){   
            while( (clock() - sound_start_clock) / CLOCKS_PER_SEC <= hogwarts_spb / 1.85){
            }
        }
        else{
            while( (clock() - sound_start_clock) / CLOCKS_PER_SEC <= hogwarts_spb){
            }
        }

        // std::cout << (clock() - sound_start_clock) / CLOCKS_PER_SEC << std::endl;

    }

    // 演奏終了
    pwm_set_gpio_level( PIN_Speaker_PWM, ( sound_rest ) );

}


void play_mario(){
    
    //使用する音の周波数の宣言    
    const double sound_rest = 0;
    const double sound_G4 = 391.995;
    const double sound_Gs4 = 415.305;
    // const double sound_A4 = 440; 　// ラ
    const double sound_As4 = 466.164;
    const double sound_B4 = 493.883;
    const double sound_C5 = 523.251;
    const double sound_D5 = 587.330;
    const double sound_Ds5 = 622.254;
    const double sound_E5 = 659.255;
    const double sound_F5 = 698.456;
    const double sound_G5 = 783.991;
    
    const double mario_bps =360.0 / 60;
    const double mario_spb = 1 / mario_bps;

    // メロディーを配列で作成
    const std::initializer_list<double> mario_melody{ sound_G4, sound_C5, sound_E5, sound_G4, sound_C5, sound_E5, sound_G4, sound_G4, sound_G4, sound_E5, sound_E5, sound_Gs4, sound_C5, sound_Ds5, sound_Gs4, sound_C5, sound_Ds5, sound_Gs4, sound_Gs4, sound_Gs4, sound_E5, sound_E5, sound_As4, sound_D5, sound_F5, sound_As4, sound_D5, sound_F5, sound_As4, sound_As4, sound_As4, sound_rest, sound_B4, sound_rest, sound_B4, sound_rest, sound_B4, sound_C5, sound_C5, sound_C5 };


    auto mario_melody_now_itr =mario_melody.begin();


    for(uint8_t mario_melody_order = 0; mario_melody_order < mario_melody.size(); mario_melody_order++){

        double sound_start_clock = clock();
        
        double mario_melody_now = *mario_melody_now_itr;

        if(mario_melody_now == sound_rest){
            pwm_set_gpio_level( PIN_Speaker_PWM, ( sound_rest ) );
        }

        else{
            static const uint32_t Raspberry_pi_clock = 125000000;
            static double speaker_duty = 0.50;

            uint16_t speaker_pwm_wrap = (Raspberry_pi_clock / (mario_melody_now * speaker_pwm_clkdiv)) - 1;

            pwm_config_set_wrap( &speaker_pwm_slice_config, speaker_pwm_wrap );
            pwm_init( speaker_pwm_slice_num, &speaker_pwm_slice_config, true );

            pwm_set_gpio_level( PIN_Speaker_PWM, ( speaker_pwm_wrap * speaker_duty ) );
        }
        
        
        // 次の音
        ++mario_melody_now_itr;
        
        // 音の継続
        if ((mario_melody_order >= 33) && (mario_melody_order < 37)){   
            while( (clock() - sound_start_clock) / CLOCKS_PER_SEC <= mario_spb / 1.95){
            }
        }
        else{
            while( (clock() - sound_start_clock) / CLOCKS_PER_SEC <= mario_spb){
            }
        }

        // std::cout << (clock() - sound_start_clock) / CLOCKS_PER_SEC << std::endl;

    }

    // 演奏終了
    pwm_set_gpio_level( PIN_Speaker_PWM, ( sound_rest ) );

}


int main(){
    speaker_init();
    play_starwars();
    sleep_ms(1000);
    play_windows7();
    sleep_ms(1000);
    play_hogwarts();
    sleep_ms(1000);
    play_mario();
}

