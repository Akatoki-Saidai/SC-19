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
    UART uart_spresense(TX(4), RX(5), 115200_hz);  // Spresenseとの通信を行うUART
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
    GPIO<In> para_separate(Pin(17), Pull::Up);  // パラシュート分離の検知用ピン (分離したらHigh)
    printf("8");
    // GPIO18は未使用
    HCSR04 hcsr04(Pin(28), Pin(19));  // 超音波センサHCSR04
    printf("9");
    Motor1 motor_right(PWM(20), PWM(21));  // 右のモーター
    printf("a");
    Speaker speaker(Pin(22));  // スピーカー
    printf("b");
    GPIO<In> usb_conect(Pin(24));  // USBの接続を確認
    LED led_red(Pin(27));    // 照度センサ搭載の赤色LED
    printf("c");
    NJL5513R njl5513r(ADC(26), led_red, led_green);  // 照度センサnjl5513r
    printf("d");
    // GPIO28は上記のように超音波センサ
    PicoTemp pico_temp;
    VsysVoltage vsys;
    LED led_pico(Pin(25));  // pico内蔵LED


    BME280 bme280(i2c_bme_bno);  // 温湿度気圧センサのBME280
    printf("e");
    BNO055 bno055(i2c_bme_bno);  // 9軸センサのBNO055;
    printf("f");
    Motor2 motor(motor_left, motor_right);  // 左右のモーター
    printf("g");
    SD sd;  // SDカード
    printf("h");
    Flush flush;

    // print関数を設定
    set_print = [&](const std::string & message)
    {
        std::cout << message << std::endl;
        sd.write(message);
        flush.write(message);
    };
    

/***** loop *****/
    while (true)
    {
        try
        {
            
        flush.write("aiueo\n");
        flush.write("kakikukeko\n");
        // flush.write(Binary(std::array<uint8_t, 250U>{}));
        sleep(1_s);
        if (yobi_spresense.read())
            flush.print();
        if (!para_separate.read())
            flush.clear();

        /***** 受信 *****/

        auto bme_data = bme280.read();  // BME280(温湿圧)から受信
    printf("i");

        auto bno_data = bno055.read();  // BNO055(9軸)から受信
    printf("j");

        auto hcsr_data = hcsr04.read();  // HCSR04(超音波)から受信
    printf("k");

        auto njl_data = njl5513r.read();  // NJL5513R(照度)から受信
    printf("l");


        /***** 表示 *****/

        Pressure<Unit::Pa> pressure = std::get<0>(bme_data);  // 気圧
    printf("m");
        Humidity<Unit::percent> humidity = std::get<1>(bme_data);  // 湿度
    printf("n");
        Temperature<Unit::K> temperature = std::get<2>(bme_data);  // 気温
    printf("o");
        print("pressure:%f Pa, humidity:%f %, temperature:%f K\n", pressure, humidity, temperature);
    printf("p");

        Acceleration<Unit::m_s2> line_acceleration = std::get<0>(bno_data);  // 線形加速度
    printf("q");
        Acceleration<Unit::m_s2> gravity_acceleration = std::get<1>(bno_data);  // 重力加速度
    printf("r");
        MagneticFluxDensity<Unit::T> magnetic = std::get<2>(bno_data);  // 磁束密度
    printf("s");
        AngularVelocity<Unit::rad_s> gyro = std::get<3>(bno_data);  // 角加速度
        print("line acceleration x:%f, y:%f, z:%f m/s2\n", line_acceleration.x(), line_acceleration.y(), line_acceleration.z());
        print("gravity acceleration x:%f, y:%f, z:%f m/s2\n", gravity_acceleration.x(), gravity_acceleration.y(), gravity_acceleration.z());
        print("magnetic x:%f, y:%f, z:%f T\n", magnetic.x(), magnetic.y(), magnetic.z());
        print("gyro x:%f, y:%f, z:%f rad/s2\n", gyro.x(), gyro.y(), gyro.z());

        print("kyori:%f m\n", hcsr_data);

        print("syoudo:%f lx\n\n", njl_data);

        auto pico_temp_data = pico_temp.read();
        print("temp : %f degC\n", pico_temp_data);
    
        auto vsys_data = vsys.read();
        print("vsys : %f V\n", vsys_data);



        /***** 動作 *****/

        led_red.on();  // 赤色LEDを点ける
    printf("t");
        // sleep(1_s);
        led_green.on();  // 緑色LEDを点ける
    printf("u");
        // sleep(1_s);
        led_red.off();
    printf("v");
        led_green.off();
    printf("w");
    led_pico.on();
    // sleep(1_s);
    led_pico.off();

        // speaker.play_windows7();  // スターウォーズを再生
    printf("x");

    // while(true)
        motor.forward(1.0);  // 前に進む
    printf("y");
        // sleep(1_s);
        motor.right(1.0);  // 右に進む
    printf("z");
        // sleep(1_s);
        motor.forward(-1.0);  // 後ろに進む
    printf("finish");

        sd.write("testtest\nsd card ni kakikomi\n");  // SDカードに書き込み

        if (para_separate.read() == true)
            print("bunri ok!\n");
        else 
            print("bunri mada\n");
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
    std::cerr << e.what() << std::endl;
    sleep(100_ms);
    exit(1);
}
}

}

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("init_ok\n");
    sc::main();
}