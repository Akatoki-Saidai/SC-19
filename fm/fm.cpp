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

int main()
{
    try
    {

    // ************************************************** //
    //                       setup                        //
    // ************************************************** //
        UART uart_twelite(TX(0), RX(1), 115200_hz);  // TWELITEとの通信を行うUART
        GPIO<In> yobi_spresense(Pin(2), Pull::Down);  // Spresenseとの通信を行う予備のピン
        GPIO<Out> yobi_twelite(Pin(3));  // TWELITEに出力する予備の通信ピン
        UART uart_spresense(TX(4), RX(5), 31250_hz);  // Spresenseとの通信を行うUART
        I2C i2c_bme_bno(SDA(6), SCL(7));  // BMEとBNOからの受信を行うI2C
        LED led_green(Pin(8));  // 照度センサ搭載の緑色LED
        // GPIO 9 はSDカード
        Motor1 motor_left(PWM(11), PWM(10));  // 左のモーター
        // GPIO 12~15 はSDカード
        // GPIO16は未使用
        GPIO<In> para_separate(Pin(17), Pull::Up);  // パラシュート分離の検知用ピン (分離したらHigh(1))
        // GPIO18は未使用
        HCSR04 hcsr04(Pin(28), Pin(19));  // 超音波センサHCSR04
        Motor1 motor_right(PWM(20), PWM(21));  // 右のモーター
        Speaker speaker(Pin(22));  // スピーカー
        GPIO<In> usb_conect(Pin(24));  // USBの接続を確認
        LED led_pico(Pin(25));  // pico内蔵LED
        LED led_red(Pin(27));    // 照度センサ搭載の赤色LED
        NJL5513R njl5513r(ADC(26), led_red, led_green);  // 照度センサnjl5513r
        PicoTemp pico_temp;
        VsysVoltage vsys;
        BME280 bme280(i2c_bme_bno);  // 温湿度気圧センサのBME280
        BNO055 bno055(i2c_bme_bno);  // 9軸センサのBNO055;
        Motor2 motor(motor_left, motor_right);  // 左右のモーター
        SD sd;  // SDカード
        Flush flush;
        Spresense spresense(uart_spresense);
        Twelite twelite(uart_twelite);

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
            try {twelite.write(0x78, message);} catch(const std::exception& e){printf(e.what());}
        };
        
        // 標高の基準となる気圧を設定
        try
        {
            auto b0 = bme280.read();
            sleep(1_ms);
            auto b1 = bme280.read();
            sleep(1_ms);
            auto b2 = bme280.read();
            Altitude<Unit::m>::set_origin(average(std::get<0>(b0), std::get<0>(b1), std::get<0>(b2)), average(std::get<2>(b0), std::get<2>(b1), std::get<2>(b2)));
        }
        catch(const std::exception& e){printf(e.what());}

        fase = Fase::Wait;

        absolute_time_t recent_successful = get_absolute_time();  // エラーが出続けている時間を測るために使う．
        bool is_success = true;  // エラーが出ずに成功したか

        const absolute_time_t start_time = get_absolute_time();//開始時刻
        led_pico.on();
        led_red.off();
        led_green.off();
        speaker.play_windows7();

    // ************************************************** //
    //                        loop                        //
    // ************************************************** //
        while (true) { 
        try {
        while (true)
        {
            try
            {
                try
                {
                    led_pico.on();
                    print("\nfase : %d\n", int(fase));  // フェーズを表示
                    spresense.time();  // タイムスタンプを表示
                    vsys.read();  // 電源電圧を表示
                    if (is_success)  // もし，前回のループがうまくいったなら
                    {
                        recent_successful = get_absolute_time();
                    }
                    is_success = true;
                }
                catch(const std::exception& e){print(e.what());}
                
                switch (fase)
                {
                    // ************************************************** //
                    //                     待機フェーズ                    //
                    // ************************************************** //
                    case Fase::Wait:
                    {
                        try
                        {
                            sleep_ms(100);//後から追加
                            led_red.off();
                            led_green.off();

                            //条件1：開始から４分以上　→落下フェーズへ
                            if((absolute_time_diff_us(start_time, get_absolute_time())>240*1000*1000))
                            {
                                fase=Fase::Fall;
                                print("Shifts to the falling phase under condition 1\n");  // 条件1で落下フェーズに移行します
                                break;
                            }
                            //条件2：エラー２分以上　→落下フェーズへ
                            if (absolute_time_diff_us(recent_successful, get_absolute_time()) > 120*1000*1000)
                            {
                                fase=Fase::Fall;
                                print("Shifts to the falling phase under condition 2\n");  // 条件2で落下フェーズに移行します
                                break;
                            }
                            try
                            {
                                //条件3：開始から２分以上＆高度５ｍ以下　→落下フェーズへ
                                auto bme_data = bme280.read();  // BME280(温湿圧)から受信
                                Pressure<Unit::Pa> pressure = std::get<0>(bme_data);  // 気圧
                                Temperature<Unit::degC> temperature = std::get<2>(bme_data);  // 気温
                                Altitude<Unit::m> altitude(pressure, temperature);
                                if((absolute_time_diff_us(start_time, get_absolute_time())>120*1000*1000)&&(altitude<5_m))
                                {
                                    fase=Fase::Fall;
                                    print("Shifts to the falling phase under condition 3\n");  // 条件3で落下フェーズに移行します
                                    break;
                                }
                            }
                            catch(const std::exception& e) { print(e.what()); is_success = false; led_pico.off(); }
                            try
                            {
                                //条件4：照度によりキャリア展開検知&&自由落下　→落下フェーズへ
                                auto njl_data = njl5513r.read();
                                auto bno_data = bno055.read();
                                if((njl_data>2500_lx)&&(is_free_fall(std::get<0>(bno_data), std::get<1>(bno_data))))
                                {
                                    fase=Fase::Fall;
                                    print("Shifts to the falling phase under condition 4\n");  // 条件4で落下フェーズに移行します
                                    break;
                                }
                            }
                            catch(const std::exception& e) { print(e.what()); is_success = false; led_pico.off(); }
                        }
                        catch(const std::exception& e) { print(e.what()); is_success = false; led_pico.off(); }
                        break;  // 保険のbreak
                    }

                    // ************************************************** //
                    //                     落下フェーズ                    //
                    // ************************************************** //
                    case Fase::Fall:
                    {
                        try
                        {   
                            led_red.off();
                            led_green.on();

                            //条件1：電源オンから5分以上経過　→遠距離フェーズへ
                            if(absolute_time_diff_us(start_time, get_absolute_time())>300*1000*1000)
                            {
                                fase=Fase::Ldistance;
                                print("Shifts to the long distance phase under condition 1\n");  // 条件1で遠距離フェーズに移行します
                            }
                            //条件2：エラーが2分以上続く　→遠距離フェーズへ
                            if(absolute_time_diff_us(recent_successful, get_absolute_time()) > 120*1000*1000){
                                fase=Fase::Ldistance;
                                print("Shifts to the long distance phase under condition 2\n");  // 条件2で遠距離フェーズに移行します
                            }

                            try
                            {
                                auto bme_data = bme280.read();  // BME280(温湿圧)から受信
                                Pressure<Unit::Pa> pressure = std::get<0>(bme_data);  // 気圧
                                Temperature<Unit::degC> temperature = std::get<2>(bme_data);  // 気温
                                Altitude<Unit::m> altitude(pressure, temperature);
                                print("altitude:%f\n",double(altitude));
                                if(altitude<5_m)  //地面からの標高が5m以内
                                {
                                    auto bno_data = bno055.read();  // BNO055(9軸)から受信
                                    //条件3：静止　→遠距離フェーズへ
                                    if(is_stationary(std::get<0>(bno_data)))
                                    {
                                        fase=Fase::Ldistance;
                                        print("Shifts to the long distance phase under condition 3\n");  // 条件3で遠距離フェーズに移行します
                                    } else {
                                        break;
                                    }
                                }
                            }
                            catch(const std::exception& e) { print(e.what()); is_success = false; led_pico.off(); }

                            if (fase == Fase::Ldistance)
                            {
                                speaker.play_hogwarts();  //念のため待機しておく

                                if(para_separate.read() == false)  //パラシュートが取れていない場合、動いてみる？
                                {
                                    print("para:toretenai\n");
                                    motor.forward(1.0);
                                    sleep(1_s);
                                    motor.forward(-1.0);
                                    sleep(1_s);
                                    motor.forward(0);
                                }
                                
                                auto bno_data = bno055.read();  // BNO055(9軸)から受信
                                Acceleration<Unit::m_s2> gravity_acceleration = std::get<1>(bno_data);//重力加速度取得
                                //Z軸の重力加速度が正かどうかで機体の体制修正
                                if(gravity_acceleration.z()<3_m_s2)//z軸が負なら正常
                                {
                                    print("muki:atteru\n");
                                } else {
                                    print("muki:hantai\n");
                                    motor.forward(1.0);//じたばたして体制修正できるかな？
                                    sleep(1_s); 
                                    motor.forward(0);
                                    motor.right(1.0); 
                                    sleep(1_s);
                                    motor.right(0); 
                                    motor.left(1.0); 
                                    sleep(1_s);
                                    motor.left(0);
                                }
                                break;
                            }
                        }
                        catch(const std::exception& e) { print(e.what()); is_success = false; led_pico.off(); }
                        break;  // 保険のbreak
                    }

                    // ************************************************** //
                    //                   遠距離フェーズ                    //
                    // ************************************************** //
                    case Fase::Ldistance:
                    {
                        try
                        {
                            led_green.off();
                            led_red.on();
                            //------ちゃんと動くか確認するためのコード-----
                            auto bno_data = bno055.read();
                            auto gps_data = spresense.gps();
                            MagneticFluxDensity<sc::Unit::T> magnetic = std::get<2>(bno_data);
                            double t_lon = 139.607348;//ゴールの経度(自分たちで決めて書き換えてね)
                            double t_lat = 35.863522;//ゴールの緯度
                            double m_lon = double(std::get<1>(gps_data));//自分の経度(ここはGPSで手に入れたものが入るように書き換えて)
                            double m_lat = double(std::get<0>(gps_data));//自分の緯度
                            double t_lon_rad = deg_to_rad(t_lon);
                            double t_lat_rad = deg_to_rad(t_lat);
                            double m_lon_rad = deg_to_rad(m_lon);
                            double m_lat_rad = deg_to_rad(m_lat);
                            //-------------------------------------------

                            //機体の正面のベクトルを作る.ただし、bnoの都合上、後ろがxの正の向きで左がyの正の向き、下がzの正の向きとなっている
                            // Vector3<double> front_vetor_basic = (-1.0, 0.0, 0.0);//機体正面の単位ベクトル

                            //北を見つける
                            Vector3<double> North_vector(double(magnetic.x()),double(magnetic.y()),0);//磁気センサから求める北の向き
                            // Vector3<double> North_vector_basic = Normalization(North_vector);//正規化
                            double North_angle_rad;//後で使う北の角度

                            // 北がBnoの座標軸において何度回転した位置にあるか求める
                            // 但しθは[0,2Pi)とした
                            North_angle_rad = atan2(double(magnetic.y()),double(magnetic.x()));
                            if(North_angle_rad < 0)
                            {
                                North_angle_rad += 2 * PI;
                            }

                            printf("%f\n",North_angle_rad);
                            
                            //スタート地点からゴールまでのベクトルを求める
                            //--------------一旦動くかわからんが書いてみる--------------

                            double distance = distance_sphere(t_lon_rad,t_lat_rad,m_lon_rad,m_lat_rad);//ゴールと自分の距離を求める
                            double distance_vertical = distance_sphere(m_lon_rad,t_lat_rad,m_lon_rad,m_lat_rad);//縦の距離を求める(経度を同じにすることで)
                            double distance_horizontal = distance_sphere(t_lon_rad,m_lat_rad,m_lon_rad,m_lat_rad);//横の長さを求める(緯度を同じにすることで)

                            //ここが結構怪しい
                            //自分の緯度のほうが高い場合、南向きに進む必要があるため、－符号をかける(北が正のため)
                            if(m_lat_rad > t_lat_rad)
                            {
                                distance_vertical = -1*distance_vertical;
                                // return distance_vertical;
                            }
                            //自分の経度のほうが小さい場合、東に向かう必要があるため(以下略)
                            if(m_lon_rad < t_lon_rad)
                            {
                                distance_horizontal = -1*distance_horizontal;
                                // return distance_horizontal;
                            }
                            
                            print("%f\n",distance);
                            printf("%f\n",distance_vertical);
                            printf("%f\n",distance_horizontal);
                            
                            Vector3<double> direction(distance_vertical,distance_horizontal,0);
                            //----------------------------
                            Vector3<double> direction_vector_1(direction.x(),direction.y(),0);//東西南北を基底としたベクトルでベクトルを表現(北がx軸,西がy軸)
                            
                            //ベクトルのprintわかんなかったからChatGPTさんに出力してもらったよ。間違ってたら直してほしい
                            std::cout << "(" << direction_vector_1.x() << ", " << direction_vector_1.y() << ", " << direction_vector_1.z() << ")" << std::endl;
                            
                            Vector3<double> direction_vector_2 = Rotation_clockwise_xy(direction_vector_1,Latitude<sc::Unit::rad>(North_angle_rad));//東西南北の基底から機体のxyを基底とした座標に回転.
                            double direction_angle_rad;
                            
                            //ここも
                            std::cout << "(" << direction_vector_2.x() << ", " << direction_vector_2.y() << ", " << direction_vector_2.z() << ")" << std::endl;

                            direction_angle_rad = atan2(direction_vector_2[1],direction_vector_2[0]);

                            printf("%f\n",direction_angle_rad);

                            //front_vectorと呼べるものが(1,0,0)の場合
                            if(direction_angle_rad < 0)
                            {
                                direction_angle_rad += 2 * PI;
                            }

                            //front_vectorと呼べるものが(-1,0,0)の場合
                            // direction_angle_rad = direction_angle_rad + PI;//正面がxの負の向きなので180°回転

                            double direction_angle_degree = rad_to_deg(direction_angle_rad);
                            print("%f\n",direction_angle_degree);
                            //ここからdirection_angleをもとに機体を動かす
                            //一旦SC-17のコードを引っ張ってきたよ
                            //sleep_msよりsleep使ったほうがいい?
                            if(direction_angle_degree < 45 || direction_angle_degree > 315)
                            {
                                print("forward\n");
                                // forward(1);
                                // sleep_ms(3000);
                                // forward(0.6);
                                // sleep_ms(1000);
                                // forward(0.2);
                                // sleep_ms(1000);
                                // forward(0);
                                // sleep_ms(100);
                                motor.forward(1.0);
                                sleep_ms(3000);
                                motor.forward(0.6);
                                sleep_ms(1000);
                                motor.forward(0.2);
                                sleep_ms(1000);       
                                motor.forward(0);
                                sleep_ms(100);
                            }else if((direction_angle_degree) <135){
                                print("left\n");
                                // left(0.5);
                                // sleep_ms(300);
                                // left(0);
                                // sleep_ms(100);
                                motor.left(0.5);
                                sleep_ms(300);
                                motor.left(0);
                                sleep_ms(100);
                            }else if(direction_angle_degree < 180){
                                print("sharp_left\n");
                                // left(0.5);
                                // sleep_ms(600);
                                // left(0);
                                // sleep_ms(100);
                                motor.left(0.5);
                                sleep_ms(600);
                                motor.left(0);
                                sleep_ms(100);
                            }else if(direction_angle_degree > 225){
                                print("right\n");
                                // right(0.5);
                                // sleep_ms(300);
                                // right(0);
                                // sleep_ms(100);
                                motor.right(0.5);
                                sleep_ms(300);
                                motor.right(0);
                                sleep_ms(100);
                            }else{
                                print("sharp_right\n");
                                // right(0.5);
                                // sleep_ms(600);
                                // right(0);
                                // sleep_ms(100);
                                motor.right(0.5);
                                sleep_ms(600);
                                motor.right(0);
                                sleep_ms(100);
                            }

                            if(distance < 5) //条件1：ゴールとの距離が5ｍ未満　→近距離フェーズへ
                            {
                                fase=Fase::Sdistance;
                                print("Shifts to the short distance phase under condition 1\n");  // 条件1で近距離フェーズに移行します
                                speaker.play_starwars();
                                break;
                            }
                        }
                        catch(const std::exception& e)
                        {
                            print(f_err(__FILE__, __LINE__, e, "era-desu"));
                            is_success = false;
                            led_pico.off();
                            // もしエラーがでるなら
                            try
                            {
                                motor.run(0.6, 1.0);  // とりあえず進んでみる
                                sleep(5_s);
                                motor.brake();
                            }
                            catch(const std::exception& e){print(e.what());}                            
                        }
                        break;  // 保険のbreak
                    }

                    //5m 以下になったら近距離フェーズに移行

                    // ************************************************** //
                    //                   近距離フェーズ                    //
                    // ************************************************** //
                    case Fase::Sdistance:
                    {
                        try
                        {
                            led_red.on();
                            led_green.on();
                            auto camera_data = spresense.camera();
                            
                            if(camera_data == Cam::Center)//ゴールがカメラの真ん中
                            {
                                //少し進む
                                motor.forward(1.0);
                                sleep(1_s);
                                motor.forward(0);
                                //超音波でゴール検知　→ゴール    
                                double hcsr_data_average = 0;
                                for(int i=0;i<10;++i)//超音波の値の平均
                                {
                                    hcsr_data_average += double(hcsr04.read());
                                }
                                hcsr_data_average/=10.0;
                                if(hcsr_data_average<0.2)//0.2m以内でゴール
                                {
                                    speaker.play_mario();
                                    print("goal\n");
                                    while(true)
                                    {
                                        ;
                                    }
                                }
                            }
                            else if(camera_data == Cam::Right)//ゴールがカメラの右
                            {
                                motor.right(1.0); 
                                sleep(1_s);
                                motor.right(0); 
                                break;
                            }
                            else if(camera_data == Cam::Left)//ゴールがカメラの左
                            {
                                motor.left(1.0); 
                                sleep(1_s);
                                motor.left(0); 
                                break;
                            }
                            else//ゴールがみつからない
                            {
                                motor.right(1.0); 
                                sleep(1_s);
                                motor.right(0); 
                                break;
                            }
                        }
                        catch(const std::exception& e) { print(e.what()); is_success = false; led_pico.off(); }
                        break;  // 保険のbreak
                    }

                    // ************************************************** //
                }
            }
            catch(const std::exception& e){print(e.what());}
        }
        } catch(const std::exception& e){print(e.what());}
        }
    }
    catch(const std::exception& e)
    {
        // エラー時の出力
        print(e.what());
        print("\nOut of the loop\n");
    }
}


}  // namespace sc

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("init_ok\n");
    while(true) 
        sc::main();
}
