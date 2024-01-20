#ifndef SC19_PICO_SC_MEASUREMENT_HPP_
#define SC19_PICO_SC_MEASUREMENT_HPP_

/**************************************************
 * 測定値に関するコードです
 * このファイルは，measurement.cppに書かれている関数の一覧です
 * 
 * このファイルでは，気温や加速度などの測定値，緯度や経度などの位置に関するコードが宣言されています．
**************************************************/

//! @file measurement.hpp
//! @brief 測定値とその変換

#include "sc_basic.hpp"

#include <math.h>

#ifndef M_PI  // もし，円周率(M_PI)が定義されていなかったら，円周率を定義する
    #define M_PI 3.14159265358979323846
#endif

namespace sc
{

// //! @brief 単位の親クラス
// class _Unit
// {
//     //! @brief double型への変換を強制する仮想関数
//     //! @note _Unit型を継承したクラスでは，以下のdouble型に変換する関数を定義しないとエラーになります．
//     virtual operator double() const = 0;
// };

// //! @brief 温度(℃)
// class _degC : _Unit
// {
//     const double _degC_value;
// public:
//     //! @brief 温度(℃)のセット
//     _degC(double degC_value):
//         _degC_value(degC_value) {}
    
//     //! @brief 温度(℃)をdouble型に変換
//     operator double() const override {return _degC_value;}
// };



// //! @brief x座標の値やx成分の値
// class X
// {
//     double _x_value;
// public:
//     //! @brief xの値をセット
//     //! @param x_value x座標の値やx成分の値
//     X(double x_value):
//         _x_value(x_value) {}

//     //! @brief X型をdouble型に変換
//     operator double() {return _x_value;}
// };

// //! @brief Y座標の値やx成分の値
// class Y
// {
//     double _y_value;
// public:
//     //! @brief Yの値をセット
//     //! @param y_value x座標の値やx成分の値
//     Y(double y_value):
//         _y_value(y_value) {}

//     //! @brief Y型をdouble型に変換
//     operator double() {return _y_value;}
// };

// //! @brief Z座標の値やx成分の値
// class Z
// {
//     double _z_value;
// public:
//     //! @brief Zの値をセット
//     //! @param z_value z座標の値やx成分の値
//     Z(double z_value):
//         _z_value(z_value) {}

//     //! @brief Z型をdouble型に変換
//     operator double() {return _z_value;}
// };

// //! @brief 原点からの距離，絶対値
// class R
// {
//     double _r_value;
// public:
//     //! @brief 絶対値をセット
//     //! @param r_value 原点からの距離や絶対値
//     R(double r_value):
//         _r_value(z_value) {}

//     //! @brief 絶対値をdouble型に変換
//     operator double() {return _r_value;}
// };

}

#endif  // SC19_PICO_SC_MEASUREMENT_HPP_