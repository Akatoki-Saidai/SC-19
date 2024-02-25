//遠距離フェーズのプログラムを書くところです

//備忘録:値がうまく帰ってこなかったとき用のコードも書くべき(SC17)if(measurement.lat==-1024.0 || measurement.lon==-1024.0) return;みたいな
//経度が西経、東経どうやって与えられるかわからんかったのでdirectionベクトルを作る際にミスってるかも

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

//球面上の距離を測る関数(一旦)
double distance_sphere(double t_lon,double t_lat,double m_lon,double m_lat){
    double cos_n = sin(t_lat)*sin(m_lat) + cos(t_lat)*cos(m_lat)*cos(t_lon - m_lon);//なす角のcos
    const double R = 6371.0;
    double distance = R * acos(cos_n);
    return distance;
}


//------------------------------------------



void long_fase(const BME280& bme280, const BNO055& bno055, const HCSR04& hcsr04, const NJL5513R& njl5513r, const GPIO<In>& para_separate, const GPIO<In>& usb_conect, const Motor2& motor, const Speaker& speaker, const LED& led_green, const LED& led_red, const LED& led_pico)
{
    //------ちゃんと動くか確認するためのコード-----
    Vector3<double> magnetic = (0.0,0.0,0.0);
    double t_lon = 0.0000;//ゴールの経度
    double t_lat = 0.0000;//ゴールの緯度
    double m_lon = 0.0000;//自分の経度
    double m_lat = 0.0000;//自分の緯度
    double t_lon_rad = deg_to_rad(t_lon);
    double t_lat_rad = deg_to_rad(t_lat);
    double m_lon_rad = deg_to_rad(m_lon);
    double m_lat_rad = deg_to_rad(m_lat);
    //-------------------------------------------

    //機体の正面のベクトルを作る.ただし、bnoの都合上、後ろがxの正の向きで左がyの正の向き、下がzの正の向きとなっている
    // Vector3<double> front_vetor_basic = (-1.0, 0.0, 0.0);//機体正面の単位ベクトル

    //北を見つける
    Vector3<double> North_vector = (magnetic.x(),magnetic.y(),0);//磁気センサから求める北の向き
    // Vector3<double> North_vector_basic = Normalization(North_vector);//正規化
    double North_angle_rad;//後で使う北の角度

    // 北がBnoの座標軸において何度回転した位置にあるか求める
    // 但しθは[0,2Pi)とした
    North_angle_rad = atan2(magnetic.y(),magnetic.x());
    if(North_angle_rad < 0){
        North_angle_rad += 2 * PI;
    }

    printf("%f\n",North_angle_rad);
    
    //スタート地点からゴールまでのベクトルを求める
    //--------------一旦動くかわからんが書いてみる--------------

    double distance = distance_sphere(t_lon_rad,t_lat_rad,m_lon_rad,m_lat_rad);//ゴールと自分の距離を求める
    double distance_vertical = distance_sphere(m_lon_rad,t_lat_rad,m_lon_rad,m_lat_rad);//縦の距離を求める(経度を同じにすることで)
    double distance_horizontal = distance_sphere(t_lon_rad,m_lat_rad,m_lon_rad,m_lat_rad);//横の長さを求める(緯度を同じにすることで)

    //自分の緯度のほうが高い場合、南向きに進む必要があるため、－符号をかける(北が正のため)
    if(m_lat_rad > t_lat_rad){
        distance_vertical = -1*distance_vertical;
        return distance_vertical;
    }
    //自分の経度のほうが小さい場合、東に向かう必要があるため(以下略)
    if(m_lon_rad < t_lon_rad){
        distance_horizontal = -1*distance_horizontal;
        return distance_horizontal;
    }

    Vector3<double> direction = (distance_vertical,distance_horizontal,0);
    //----------------------------

    Vector3<double> direction_vector_1 = (direction.x(),direction.y(),0);//東西南北を基底としたベクトルでベクトルを表現(北がx軸,西がy軸)
    Vector3<double> direction_vector_2 = Rotation_counter_xy(direction_vector_1,North_angle_rad);//東西南北の基底から機体のxyを基底とした座標に回転.
    double direction_angle_rad;

    direction_angle_rad = atan2(direction_vector_2[1],direction_vector_2[0]);
    direction_angle_rad = direction_angle_rad - PI;//正面がxの負の向きなので180°回転
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