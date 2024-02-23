//遠距離フェーズのプログラムを書くところです

//備忘録:値がうまく帰ってこなかったとき用のコードも書くべき(SC17)if(measurement.lat==-1024.0 || measurement.lon==-1024.0) return;みたいな


#include "fm.hpp"

namespace sc 
{
//---------使いたい関数を作るコーナー--------

//正規化する関数を作っとく
Vector3<double> Normalization(Vector3<double> vec){
    double magnitude = sqrt(pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2));
    if (magnitude != 0.0){
        vec = vec / magnitude;
    }
    return vec;
}

// ベクトルをxy平面で反時計回りに回転する関数
Vector3<double> Rotation_counter_xy(Vector3<double> vec, Latitude<Unit::rad> Radian) {
    return Vector3<double>(vec[0] * cos(Radian) - vec[1] * sin(Radian),vec[0] * sin(Radian) + vec[1] * cos(Radian),vec[2]);
}
// ベクトルをxy平面で時計回りに回転する関数
Vector3<double> Rotation_clockwise_xy(Vector3<double> vec, Latitude<Unit::rad> Radian) {
    return Vector3<double>(vec[0] * cos(Radian) + vec[1] * sin(Radian),vec[1] * cos(Radian) - vec[0] * sin(Radian) ,vec[2]);
}

//------------------------------------------

void long_fase(const BME280& bme280, const BNO055& bno055, const HCSR04& hcsr04, const NJL5513R& njl5513r, const GPIO<In>& para_separate, const GPIO<In>& usb_conect, const Motor2& motor, const Speaker& speaker, const LED& led_green, const LED& led_red, const LED& led_pico)
{

    //機体の正面のベクトルを作る.ただし、bnoの都合上、後ろがxの正の向きで左がyの正の向き、下がzの正の向きとなっている
    // Vector3<double> front_vetor_basic = (-1.0, 0.0, 0.0);//機体正面の単位ベクトル

    //北を見つける
    Vector3<double> North_vector = (magnetic.x(),magnetic.y(),0);//磁気センサから求める北の向き
    // Vector3<double> North_vector_basic = Normalization(North_vector);//正規化
    double North_angle_rad;//後で使う北の角度

    // 北が正面の基底ベクトルから何度回転した位置にあるか求める
    // 但しθは[0,2Pi)とした
    North_angle_rad = atan2(magnetic.y(),magnetic.x());
    if(North_angle_rad < 0){
        North_angle_rad += 2 * PI;
    }

    //スタート地点からゴールまでのベクトルを求める

    //ここで困ってる
    double direction_N;
    double direction_W;
    Vector3<double> direction_vector_1 = (direction_N,direction_W,0);//東西南北を基底としたベクトルでベクトルを表現(北がx軸,西がy軸)
    Vector3<double> direction_vector_2 = Rotation_counter_xy(direction_vector_1,North_angle_rad);//東西南北の基底から機体のxyを基底とした座標に回転.
    double direction_angle_rad;

    direction_angle_rad = atan2(direction_vector_2[1],direction_vector_2[0]);
    if(direction_angle_rad < 0){
        direction_angle_rad += 2 * PI;
    }

    double direction_angle_degree = rad_to_deg(direction_angle_rad);

    //ここからdirection_angleをもとに機体を動かす
    //一旦SC-17のコードを引っ張ってきたよ
    //sleep_msよりsleep使ったほうがいい?
        if(direction_angle_degree < 45 || direction_angle_degree > 315){
        printf("forward\n");
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
        break;
    }else if((direction_angle_degree) <135){
        printf("right\n");
        // right(0.5);
        // sleep_ms(300);
        // right(0);
        // sleep_ms(100);
        motor.right(0.5);
        sleep_ms(300);
        motor.right(0);
        sleep_ms(100);
        break;
    }else if(direction_angle_degree < 180){
        printf("sharp_right\n");
        // right(0.5);
        // sleep_ms(600);
        // right(0);
        // sleep_ms(100);
        motor.right(0.5);
        sleep_ms(600);
        motor.right(0);
        sleep_ms(100);
        break;
    }else if(direction_angle_degree > 225){
        printf("left\n");
        // left(0.5);
        // sleep_ms(300);
        // left(0);
        // sleep_ms(100);
        motor.left(0.5);
        sleep_ms(300);
        motor.left(0);
        sleep_ms(100);
        break;
    }else{
        printf("sharp_left\n");
        // left(0.5);
        // sleep_ms(600);
        // left(0);
        // sleep_ms(100);
        motor.left(0.5);
        sleep_ms(600);
        motor.left(0);
        sleep_ms(100);
        break;
    }
}
}