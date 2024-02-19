#include "fm.hpp"

namespace sc
{

enum class Fase 
{
    Wait,//待機
    Fall,//落下
    Ldistance,//遠距離
    Sdistance,//近距離
} fase;

void wait_fase();
void fall_fase();
void long_fase();
void short_fase();

int main()
{
    try
    {
    /***** setup *****/

        UART uart_twelite(TX(0), RX(1), 115200_hz);  // TWELITEとの通信を行うUART
        GPIO<In> yobi_spresense(Pin(2));  // Spresenseとの通信を行う予備のピン
        GPIO<Out> yobi_twelite(Pin(3));  // TWELITEに出力する予備の通信ピン
        UART uart_spresense(TX(4), RX(5), 115200_hz);  // Spresenseとの通信を行うUART
        I2C i2c_bme_bno(SDA(6), SCL(7));  // BMEとBNOからの受信を行うI2C
        LED led_green(Pin(8));  // 照度センサ搭載の緑色LED
        // GPIO 9 はSDカード
        Motor1 motor_left(PWM(11), PWM(10));  // 左のモーター
        // GPIO 12~15 はSDカード
        // GPIO16は未使用
        GPIO<In> para_separate(Pin(17), Pull::Up);  // パラシュート分離の検知用ピン (分離したらHigh)
        // GPIO18は未使用
        HCSR04 hcsr04(Pin(28), Pin(19));  // 超音波センサHCSR04
        Motor1 motor_right(PWM(20), PWM(21));  // 右のモーター
        Speaker speaker(Pin(22));  // スピーカー
        GPIO<In> usb_conect(Pin(24));  // USBの接続を確認
        LED led_red(Pin(27));    // 照度センサ搭載の赤色LED
        NJL5513R njl5513r(ADC(26), led_red, led_green);  // 照度センサnjl5513r
        // // GPIO28は上記のように超音波センサ
        PicoTemp pico_temp;
        VsysVoltage vsys;
        GPIO<Out> led_pico(Pin(25));  // pico内蔵LED


        BME280 bme280(i2c_bme_bno);  // 温湿度気圧センサのBME280
        BNO055 bno055(i2c_bme_bno);  // 9軸センサのBNO055;
        Motor2 motor(motor_left, motor_right);  // 左右のモーター
        SD sd;  // SDカード

        // print関数を設定
        set_print = [&](const std::string & message)
        {
            std::cout << message << std::endl;
            sd.write(message);
        };
        

    /***** loop *****/
        while (true)
        {
            //それぞれのセンサで値取得
            //高度計算
            
            switch (fase)
            {
                case Fase::Wait: //待機フェーズ
                {
                    wait_fase(bme280, bno055, hcsr04, njl5513r, para_separate, usb_conect, motor, speaker, led_green, led_red, led_pico);
                }
                case Fase::Fall: //落下フェーズ   
                {
                    fall_fase(bme280, bno055, hcsr04, njl5513r, para_separate, usb_conect, motor, speaker, led_green, led_red, led_pico);
                }
                case Fase::Ldistance: //遠距離フェーズ
                {
                    long_fase(bme280, bno055, hcsr04, njl5513r, para_separate, usb_conect, motor, speaker, led_green, led_red, led_pico);
                }
                case Fase::Sdistance://近距離フェーズ
                {
                    short_fase(bme280, bno055, hcsr04, njl5513r, para_separate, usb_conect, motor, speaker, led_green, led_red, led_pico);
                }
            }

            return 0;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}


// void wait_fase()
// {
//     if(true)//照度によりキャリア展開検知　→落下フェーズへ
//     {
//         fase=Fase::Fall;
//         break;
//     }
//     else{
//         break;
//     }
// }



// void fall_fase()
// {
//     if(true)//高度から着地を検知・パラシュート分離検知　→遠距離フェーズへ
//     {
//         fase=Fase::Ldistance;
//         break;
//     }
//     else{
//         break;
//     }
// }


// void long_fase()
// {
//     if(true)//ゴールまでの距離４ｍ以内　→近距離フェーズへ
//     {
//         fase=Fase::Sdistance;
//         break;
//     }
//     else//GNSSで位置を計測しモータ駆動しゴールへ近づく
//     {
//         break;
//     }
// }


// void short_fase()
// {
//     if(true)//ゴールがカメラの真ん中
//     {
//         //少し進む
//         //超音波でゴール検知　→ゴール
//     }
//     else//角度調整
//     {
//         break;
//     }
// }




}

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("tareghieahgirefib\n");
    sc::main();
}