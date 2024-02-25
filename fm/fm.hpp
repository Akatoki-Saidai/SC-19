#ifndef SC19_PICO_FM_HPP_
#define SC19_PICO_FM_HPP_

#include "sc.hpp"

#include "hcsr04/hcsr04.hpp"
#include "bme280/bme280.hpp"
#include "bno055/bno055_BBM.hpp"
#include "njl5513r/njl5513r.hpp"
#include "sd/sd.hpp"
#include "speaker/speaker.hpp"
#include "spresense/spresense.hpp"
#include "twelite/twelite.hpp"

namespace sc 
{

//正規化する関数を作っとく
Vector3<double> Normalization(Vector3<double> vec){
    double magnitude = std::sqrt(std::pow(vec[0],2) + std::pow(vec[1],2) + std::pow(vec[2],2));
    // if (magnitude != 0.0){
    //     vec = vec / magnitude;
    // }
    if (magnitude == 0.0)
        throw std::range_error(f_err(__FILE__, __LINE__, "Divide by zero error"));  // ゼロ除算エラー
    return vec / magnitude;
}

// ベクトルをxy平面で反時計回りに回転する関数
Vector3<double> Rotation_counter_xy(Vector3<double> vec, Latitude<Unit::rad> Radian) {
    return Vector3<double>(vec[0] * cos(double(Radian)) - vec[1] * sin(double(Radian)),vec[0] * sin(double(Radian)) + vec[1] * cos(double(Radian)),vec[2]);
}
// ベクトルをxy平面で時計回りに回転する関数
Vector3<double> Rotation_clockwise_xy(Vector3<double> vec, Latitude<Unit::rad> Radian) {
    return Vector3<double>(vec[0] * cos(double(Radian)) + vec[1] * sin(double(Radian)),vec[1] * cos(double(Radian)) - vec[0] * sin(double(Radian)) ,vec[2]);
}

//球面上の距離を測る関数(一旦)
double distance_sphere(double t_lon,double t_lat,double m_lon,double m_lat){
    double cos_n = sin(t_lat)*sin(m_lat) + cos(t_lat)*cos(m_lat)*cos(t_lon - m_lon);//なす角のcos
    const double R = 6371.0;
    double distance = R * acos(cos_n);
    return distance;
}


}


#endif  // SC19_PICO_FM_HPP_
