// /**************************************************
//  * 測定値に関するコードです
//  * このファイルは，measurement.hppに名前だけ書かれている関数の中身です
//  * 
//  * このファイルでは，気温や加速度などの測定値，緯度や経度などの位置に関するコードが宣言されています．
// **************************************************/

// //! @file measurement.cpp
// //! @brief 測定値とその変換

// #include "measurement.hpp"

// namespace sc
// {

// /***** class Theta *****/

// Theta::Theta(_rad theta):
//     _theta(theta)
// {
//     if (MaxTheta < _theta)
//     {
// throw Error(__FILE__, __LINE__, "An invalid theta(polar-angle) value was entered");  // 無効な極角の値が入力されました
//     }
// }

// /***** class Latitude *****/

// Latitude::Latitude(_rad latitude):
//     _latitude(latitude)
// {
//     if (MaxLatitude < _latitude && _latitude < MinLatitude)  // MinLatitude=-0.5π=>1.5π
//     {
// throw Error(__FILE__, __LINE__, "An invalid latitude value was entered");  // 無効な緯度の値が入力されました
//     }
// }

// /***** class Altitude *****/

// Altitude::Altitude(_m altitude):
//     _altitude(altitude)
// {
//     if (_altitude < MinAltitude || MaxAltitude < _altitude)
//     {
// throw Error(__FILE__, __LINE__, "An invalid altitude value was entered");  // 無効な高度の値が入力されました
//     }
// }


// }