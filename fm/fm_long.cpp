//遠距離フェーズのプログラムを書くところです

#include "fm.hpp"

namespace sc 
{

//後で使いやすいように正規化する関数を作っとく
Vector3<double> Normalization(Vector3<double> vec)
{
    double magnitude = sqrt(pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2));
    if (magnitude != 0.0){
        vec = vec / magnitude;
    }

    return vec;
}

void long_fase(const BME280& bme280, const BNO055& bno055, const HCSR04& hcsr04, const NJL5513R& njl5513r, const GPIO<In>& para_separate, const GPIO<In>& usb_conect, const Motor2& motor, const Speaker& speaker, const LED& led_green, const LED& led_red, const LED& led_pico)
{

    //機体の正面のベクトルを作る
    //ただし、bnoの都合上、後ろがxの正の向きで左がyの正の向き、下がzの正の向きとなっている

    Vector3<double> front_vetor_basic = (-1.0, 0.0, 0.0);//機体正面の基底ベクトル

    //北を見つける

    Vector3<double> North_vector = (magnetic.x(),magnetic.y(),0);//磁気センサから求める北の向き
    // double North_vector_size = sqrt(pow(magnetic.x(), 2) + pow(magnetic.y(), 2));//上のベクトルの大きさを求める
    // Vector3<double> NOrth_vector_basic = (magnetic.x() / North_vector_size ,magnetic.y() / North_vector_size,0);//North_vectorを正規化し、北向きの基底ベクトルを作る
    Vector3<double> North_vector_basic = Normalization(North_vector);


    //スタート地点からゴールまでのベクトルを求める

    //ここで困ってる
    double direction_x;
    double direction_y;
    Vector3<double> direction_vector = (direction_x,direction_y,0);//のちの計算のために一時的にこう置いておく
    Vector3<double> direction_vector_basic = Normalization(direction_vector);


    //機体の正面と方位角を用いて回転する角度を求める

    //案1:ベクトルの外積を使ってみる

    //案2:普通にarctanで角度計算をする

}

}