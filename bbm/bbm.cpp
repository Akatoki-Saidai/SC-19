#include "bbm.hpp"

namespace sc
{

int main()
{
    try
    {
    /***** setup *****/
        printf("0");
        UART uart_twelite(TX(0), RX(1), 115200_hz);  // TWELITEとの通信を行うUART
        printf("1");
        GPIO<In> yobi_spresense(Pin(2), Pull::Down);  // Spresenseとの通信を行う予備のピン
        printf("2");
        GPIO<In> yobi_twelite(Pin(3));  // TWELITEに出力する予備の通信ピン
        printf("3");
        UART uart_spresense(TX(4), RX(5), 31250_hz);  // Spresenseとの通信を行うUART
        printf("4");
        I2C i2c_bme_bno(SDA(6), SCL(7));  // BMEとBNOからの受信を行うI2C
        printf("5");
        LED led_green(Pin(8));  // 照度センサ搭載の緑色LED
        printf("6");
        // GPIO 9 はSDカード
        Motor1 motor_left(PWM(11), PWM(10));  // 左のモーター
        printf("7");
        // GPIO 12~15 はSDカード
        // GPIO16は未使用
        GPIO<In> not_separate_para(Pin(17), Pull::Up);  // パラシュート分離の検知用ピン (分離したらHigh(1))
        printf("8");
        // GPIO18は未使用
        HCSR04 hcsr04(Pin(28), Pin(19));  // 超音波センサHCSR04
        printf("9");
        Motor1 motor_right(PWM(20), PWM(21));  // 右のモーター
        printf("a");
        Speaker speaker(Pin(22));  // スピーカー
        printf("b");
        GPIO<In> usb_conect(Pin(24));  // USBの接続を確認
        LED led_pico(Pin(25));  // pico内蔵LED
        LED led_red(Pin(27));    // 照度センサ搭載の赤色LED
        printf("c");
        NJL5513R njl5513r(ADC(26), led_red, led_green);  // 照度センサnjl5513r
        printf("d");
        PicoTemp pico_temp;
        VsysVoltage vsys;

        BME280 bme280(i2c_bme_bno);  // 温湿度気圧センサのBME280
        printf("e");
        BNO055 bno055(i2c_bme_bno);  // 9軸センサのBNO055;
        printf("f");
        Motor2 motor(motor_left, motor_right);  // 左右のモーター
        printf("g");
        SD sd;  // SDカード
        printf("h");
        Flush flush;
        Spresense sprs(uart_spresense);

        // USBでの接続時はフラッシュメモリのデータを出力
        if (usb_conect.read() == true)
        {
            flush.print();
            #ifndef NODEBUG
                flush.clear();
            #endif
        } else {
            flush.clear();  // フラッシュメモリを削除(削除しないと書き込めない)
        }

        // print関数を設定
        set_print = [&](const std::string & message)
        {
            try {std::cout << message << std::flush;} catch(const std::exception& e){printf(e.what());}
            try {flush.write(message);} catch(const std::exception& e){printf(e.what());}
            try {sd.write(message);} catch(const std::exception& e){printf(e.what());}
        };

    /***** loop *****/
        while (true)
        {
            try
            {
                /***** 受信 *****/

                auto bme_data = bme280.read();  // BME280(温湿圧)から受信

                auto bno_data = bno055.read();  // BNO055(9軸)から受信

                auto hcsr_data = hcsr04.read();  // HCSR04(超音波)から受信

                auto njl_data = njl5513r.read();  // NJL5513R(照度)から受信

                auto pico_temp_data = pico_temp.read();  // pico内蔵温度計で計測

                auto vsys_data = vsys.read();  // picoの入力電圧を計測

                /***** 表示 *****/

                Pressure<Unit::Pa> pressure = std::get<0>(bme_data);  // 気圧
                Humidity<Unit::percent> humidity = std::get<1>(bme_data);  // 湿度
                Temperature<Unit::K> temperature = std::get<2>(bme_data);  // 気温
                print("pressure:%f Pa, humidity:%f %, temperature:%f K\n", pressure, humidity, temperature);

                Acceleration<Unit::m_s2> line_acceleration = std::get<0>(bno_data);  // 線形加速度
                Acceleration<Unit::m_s2> gravity_acceleration = std::get<1>(bno_data);  // 重力加速度
                MagneticFluxDensity<Unit::T> magnetic = std::get<2>(bno_data);  // 磁束密度
                AngularVelocity<Unit::rad_s> gyro = std::get<3>(bno_data);  // 角加速度
                print("line acceleration x:%f, y:%f, z:%f m/s2\n", line_acceleration.x(), line_acceleration.y(), line_acceleration.z());
                print("gravity acceleration x:%f, y:%f, z:%f m/s2\n", gravity_acceleration.x(), gravity_acceleration.y(), gravity_acceleration.z());
                print("magnetic x:%f, y:%f, z:%f T\n", magnetic.x(), magnetic.y(), magnetic.z());
                print("gyro x:%f, y:%f, z:%f rad/s2\n", gyro.x(), gyro.y(), gyro.z());

                print("kyori:%f m\n", hcsr_data);  // 超音波距離センサ

                print("syoudo:%f lx\n\n", njl_data);  // 照度センサ

                print("temp : %f degC\n", pico_temp_data);  // pico内蔵の温度センサ

                print("vsys : %f V\n", vsys_data);  // picoの入力電圧



                /***** 動作 *****/

                led_red.on();  // 赤色LEDを点ける
                led_red.off();
                led_green.on();  // 緑色LEDを点ける
                led_green.off();
                led_pico.on();  // pico内蔵LEDを点ける
                led_pico.off();

                speaker.play_windows7();  // windows7?を再生

                motor.forward(1.0);  // 前に進む
                motor.right(1.0);  // 右に進む
                motor.forward(-1.0);  // 後ろに進む

                if (not_separate_para.read() == true)  // ピンの接続でパラシュートの分離を検知
                {
                    print("bunri ok!\n");
                } else {
                    print("bunri mada\n");
                }
                
                std::tuple<sc::Latitude<sc::Unit::deg>, sc::Longitude<sc::Unit::deg>> g = sprs.gps();
                std::cout << double(std::get<0>(g)) << " " << double(std::get<1>(g)) << std::endl;
                std::tm t = sprs.time();
                std::cout << std::asctime(&t) << std::endl;
                Cam c = sprs.camera();
                std::cout << int(c) << std::endl;

                sleep(10_ms);
            }
            catch(const std::exception& e)
            {
                print(e.what());
            }
        }

        return 0;
    }
    catch(const std::exception& e)
    {
        // エラー時の出力
        print(e.what());
        print("\nOut of the loop\n");
        sleep(100_ms);
        exit(EXIT_FAILURE);
    }
}


}  // namespace sc

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("init_ok\n");
    sc::main();
}