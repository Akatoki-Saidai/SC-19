// #ifndef SC19_PICO_SC_MEASUREMENT_HPP_
// #define SC19_PICO_SC_MEASUREMENT_HPP_

// /**************************************************
//  * 測定値に関するコードです
//  * このファイルは，measurement.cppに書かれている関数の一覧です
//  * 
//  * このファイルでは，気温や加速度などの測定値，緯度や経度などの位置に関するコードが宣言されています．
// **************************************************/

// //! @file measurement.hpp
// //! @brief 測定値とその変換

// #include "sc_basic.hpp"

// #include <cmath>

// #ifndef M_PI  // もし，円周率(M_PI)が定義されていなかったら，円周率を定義する
//     #define M_PI 3.14159265358979323846
// #endif

// namespace sc
// {

// //! @brief 極角θ
// //! @note +z軸を0として，-z軸に向かう方向が正です  0 ~ π (rad)
// //! @details 度単位かラジアン単位かが分からないので，double型への直接の変換は実装しない
// class Theta 
// {
//     const _rad _theta;
//     static inline const _rad MaxTheta{1_pi_rad};

// public:
//     //! @brief 極角θ．+z軸を0として，-z軸に向かう方向が正です  0 ~ π (rad)
//     explicit Theta(_rad theta);  // 注：explicitを外すと，Data3Dのコンストラクタがあいまいになります
    
//     //! @brief 極角θ．+z軸を0として，-z軸に向かう方向が正です  0 ~ π (rad)
//     explicit Theta(_deg theta):  // 注：explicitを外すと，Data3Dのコンストラクタがあいまいになります
//         _theta(static_cast<_rad>(theta)) {}

//     //! @brief Theta型を_rad型に変換
//     operator _rad() const
//         {return _theta;}

//     //! @brief Theta型を_deg型に変換
//     operator _deg() const
//         {return static_cast<_deg>(_theta);}
// };

// //! @brief 方位角φ
// //! @note +x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π (rad)
// //! @note 地図における方位角と反対向きなので注意！
// //! @details 度単位かラジアン単位かが分からないので，double型への直接の変換は実装しない
// class Phi 
// {
//     const _rad _phi;

// public:
//     //! @brief 方位角φ．+x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π (rad)
//     explicit Phi(_rad phi):  // 注：explicitを外すと，Data3Dのコンストラクタがあいまいになります
//         _phi(phi) {}
    
//     //! @brief 方位角φ．+x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π (rad)
//     explicit Phi(_deg phi):  // 注：explicitを外すと，Data3Dのコンストラクタがあいまいになります
//         _phi(static_cast<_rad>(phi)) {}

//     //! @brief Phi型を_rad型に変換
//     operator _rad() const
//         {return _phi;}

//     //! @brief Phi型を_deg型に変換
//     explicit operator _deg() const
//         {return static_cast<_deg>(_phi);}
// };

// // 三次元のデータ
// template<class Unit>
// class Data3D
// {
//     const Unit _x;
//     const Unit _y;
//     const Unit _z;

// public:
//     //! @brief 三次元のデータを作成する
//     //! @details -0.0は0.0に変換して扱います
//     Data3D(Unit x, Unit y, Unit z):
//         _x(static_cast<Unit>(not_minus0(static_cast<double>(x)))), _y(static_cast<Unit>(not_minus0(static_cast<double>(y)))), _z(static_cast<Unit>(not_minus0(static_cast<double>(z)))) {}

//     //! @brief 三次元のデータを作成する
//     //! @details -0.0は0.0に変換して扱います
//     //! @param r 大きさ
//     //! @param theta 極角θ．+z軸を0として，-z軸に向かう方向が正です  0 ~ π
//     //! @param phi 方位角φ．+x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π
//     Data3D(Unit r, Theta theta, Phi phi):
//         _x(static_cast<double>(r)*sin(double(_rad(theta)))*cos(double(_rad(phi)))), _y(double(r)*sin(double(_rad(theta)))*sin(double(_rad(phi)))), _z(double(r)*cos(double(_rad(theta)))) {}

//     //! @brief 3次元データのx成分を返します
//     Unit x() const noexcept 
//         {return _x;}

//     //! @brief 3次元データのy成分を返します
//     Unit y() const noexcept 
//         {return _y;}

//     //! @brief 3次元データのz成分を返します
//     Unit z() const noexcept 
//         {return _z;}

//     //! @brief 3次元データの大きさ，原点からの距離を返します
//     Unit r() const
//         {return static_cast<Unit>(std::sqrt(static_cast<double>(_x)*static_cast<double>(_x) + static_cast<double>(_y)*static_cast<double>(_y) + static_cast<double>(_z)*static_cast<double>(_z)));}

//     //! @brief 3次元データの極角θを返します
//     //! @note +z軸を0として，-z軸に向かう方向が正です  0 ~ π
//     Theta theta() const
//         {return static_cast<Theta>(static_cast<_rad>(atan(std::sqrt(static_cast<double>(_x)*static_cast<double>(_x) + static_cast<double>(_y)*static_cast<double>(_y)) / static_cast<double>(_z)) + (static_cast<double>(_z)>0.0 ? 0.0 : 2.0*M_PI)));}

//     //! @brief 3次元データの方位角φを返します．
//     //! @note +x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π
//     Phi phi() const
//         {return static_cast<Phi>(static_cast<_rad>(atan(static_cast<double>(_y)/static_cast<double>(_x)) + (static_cast<double>(_x)<0.0 ? M_PI : (static_cast<double>(_y)<0 ? 2.0*M_PI : 0.0))));}
// };
// // 推論補助  Data3D<Unit>型のUnitは単位型でなくてはならない．また，_degと_radであってはならない
// template<class Unit> Data3D(Unit, Unit, Unit) -> Data3D<AnyUnit_t<Unit>>;
// template<class Unit> Data3D(Unit, _rad, _rad) -> Data3D<AnyUnit_t<Unit>>;


// // 地球関係の定数

// constexpr double Earth_a = 6'378'137;  // 地球の長半径(赤道半径) a (m)
// constexpr double Earth_f = 1.0/298.257222101;  // 地球の扁平率 f
// constexpr double Earth_F = 1.0 / Earth_f;  // 地球の逆扁平率 F
// constexpr double Earth_b = Earth_a * (1.0 - Earth_f);  // 地球の短半径 b (m)
// inline const double Earth_e = std::sqrt(Earth_f*(2.0 - Earth_f));  // 地球の離心率 e
// // 上記の作成に当たり，以下の資料を参考にしました
// // https://psgsv.gsi.go.jp/koukyou/jyunsoku/pdf/r2/r2_shinkyu_furoku6.pdf


// //! @brief 標高
// //! @note ジオイド高(平均海面)からの高さを標高とする
// class Altitude 
// {
//     const _m _altitude;
//     static inline const _m MinAltitude = -100_m;
//     static inline const _m MaxAltitude = 1000_m;
//     static inline const _m BaseGeoidHeight = 35_m;  // 地球楕円体からジオイド(平均海面)までの高さ．日本では30~40mくらい

// public:
//     //! @brief 標高
//     //! @note ジオイド高(平均海面)からの高さを標高とする
//     Altitude(_m altitude);

//     //! @brief 標高を楕円体高に変換
//     //! @param geoid_height ジオイド高 (省略可)
//     _m to_HAE(_m geoid_height = BaseGeoidHeight)
//         {return _altitude + BaseGeoidHeight;}

//     //! @brief 標高を_m型に変換
//     explicit operator _m()
//         {return _altitude;}
// };

// //! @brief 地理緯度
// //! @note 北緯が正．-90° ~ +90°．
// class Latitude 
// {
//     const _rad _latitude;  // 緯度．0 ~ 0.5π, 1.5π ~ 2.0π
//     static inline const _rad MinLatitude{-0.5_pi_rad};  // 緯度の最小値 (rad) = 1.5π <<!
//     static inline const _rad MaxLatitude{+0.5_pi_rad};  // 緯度の最大値 (rad) = 0.5π

// public:
//     //! @brief 地理緯度
//     explicit Latitude(_rad latitude);

//     //! @brief 地理緯度
//     explicit Latitude(_deg latitude):
//         Latitude(static_cast<_rad>(latitude)) {}

//     //! @brief 極角θを地理緯度に変換
//     //! @note 緯度は-90° ~ +90°．極角θの定義と異なる．地理緯度と地心緯度にはズレを補正
//     explicit Latitude(Theta polar_angle):
//         Latitude(static_cast<_rad>(atan((1.0 / (1.0 - Earth_e*Earth_e)) * std::tan(static_cast<double>(0.5_pi_rad - static_cast<_rad>(polar_angle)))))) {}

//     //! @brief 地理緯度を極角θに変換
//     //! @note 極角は+z軸を0として，-z軸に向かう方向が正  0 ~ π．地理緯度と地心緯度のズレを補正
//     explicit operator Theta() const
//         {return static_cast<Theta>(0.5_pi_rad - static_cast<_rad>(atan((1.0 - Earth_e*Earth_e) * tan(static_cast<double>(_latitude)))));}

//     //! @brief 緯度をラジアン型に変換 (-0.5π(rad) -> 1.5π(rad)にして返す)
//     operator _rad() const
//         {return _latitude;}

//     //! @brief 緯度を度単位に変換 (-90° -> 270°にして返す)
//     operator _deg() const
//         {return static_cast<_deg>(_latitude);}

//     // //! @brief 緯度をdouble型に変換
//     // //! @note -90 ~ +90の値を返す．極角θの定義と異なる
//     // explicit operator double() const
//     //     {return static_cast<double>(static_cast<_deg>(_latitude)) - (_latitude<=1_pi_rad ? 0.0 : 360.0);}
// };

// //! @brief 経度
// //! @note 東経が正．-180° ~ +180°．
// class Longitude 
// {
//     const _rad _longitude;  // 経度．0~2π

// public:
//     //! @brief 経度
//     explicit Longitude(_rad longitude):
//         _longitude(longitude) {}

//     //! @brief 経度
//     explicit Longitude(_deg longitude):
//         _longitude(static_cast<_rad>(longitude)) {}

//     //! @brief 方位角φを経度に変換
//     explicit Longitude(Phi phi):
//         Longitude(static_cast<_rad>(phi)) {}

//     //! @brief 経度を方位角φに変換
//     //! @note 方位角は+x軸を0として，x軸からy軸に向かう方向が正  0 ~ 2π
//     explicit operator Phi() const
//         {return static_cast<Phi>(_longitude);}

//     //! @brief 経度をラジアン単位に変換 (-0.5π(rad) -> 1.5π(rad)にして返す)
//     operator _rad()
//         {return _longitude;}

//     //! @brief 経度を度単位に変換 (-90° -> 270°にして返す)
//     operator _deg() const
//         {return static_cast<_deg>(_longitude);}

//     // //! @brief 経度をdouble型に変換
//     // //! @note -180 ~ +180の値を返す
//     // explicit operator double() const
//     //     {return static_cast<double>(static_cast<_deg>(_longitude)) - (_longitude<=1_pi_rad ? 0.0 : 360.0);}
// };


// // 地球関連の計算

// //! @brief 地球の子午線曲率半径 Mφ (m)
// inline double Earth_M(Latitude lat)
//     {return Earth_a*(1.0-Earth_e*Earth_e)/pow(1.0 - Earth_e*Earth_e*std::pow(sin(double(_rad(lat))),2),1.5);}

// //! @brief 地球の卯酉(ぼうゆう)線曲率半径 Nφ (m)
// inline double Earth_N(Latitude lat) 
//     {return Earth_a/std::sqrt(1.0 - Earth_e*Earth_e*std::pow(double(_rad(lat)), 2));}

// // 上記の計算式の作成に当たり，以下の資料を参考にしました
// // https://psgsv.gsi.go.jp/koukyou/jyunsoku/pdf/r2/r2_shinkyu_furoku6.pdf
// // http://mikeo410.minim.ne.jp/cms/~othergeocentricreducedlatitude


// //! @brief 座標系
// enum CoordinateSystem
// {
//     World,  // 地心座標系  原点:地球の重心，+x:経度0°，+y:東経90°，+z:地軸(北)
//     Local,  // 局所座標系  原点:地上の基準点，+x:南，+y:東，+z:天頂
//     Body  // 機体座標系  原点:機体の重心，+x:前，+y:左，+z:上
// };


// // 機体の体勢，座標系の向き

// //! @brief 相対的な(機体の)座標系の向き．
// //! @note X,Y,Z軸の右ねじの方向への回転をそれぞれ roll, pitch, yaw とする．
// //! @note 基準となる座標系から，yaw->pitch->rollの順番に回転させたときの相対的な(機体の)座標系の向き
// template<CoordinateSystem> 
// // class Posture : private Data3D<_rad>
// // {
// // public:
// //     //! @brief 相対的な(機体の)座標系の向き．
// //     //! @note X,Y,Z軸の右ねじの方向への回転をそれぞれ roll, pitch, yaw とする．
// //     //! @note 基準となる座標系から，yaw->pitch->rollの順番に回転させたときの相対的な(機体の)座標系の向き
// //     Posture(_rad roll, _rad pitch, _rad yaw):
// //         Data3D(roll, pitch, yaw) {}

// //     //! @brief roll方向(x軸右ねじ)への回転
// //     _rad roll() const
// //         {return x();}

// //     //! @brief pitch方向(y軸右ねじ)への回転
// //     _rad pitch() const
// //         {return y();}

// //     //! @brief yaw方向(z軸右ねじ)への回転
// //     _rad yaw() const
// //         {return z();}
// // };


// // 位置
// template<CoordinateSystem> class Position;

// // // 三次元の位置
// // template<class _m>
// // class Position3D
// // {
// //     const _m _x;
// //     const _m _y;
// //     const _m _z;

// // public:
// //     //! @brief 三次元の位置データを作成する
// //     //! @details -0.0は0.0に変換して扱います
// //     Position3D(_m x, _m y, _m z):
// //         _x(static_cast<_m>(not_minus0(static_cast<double>(x)))), _y(static_cast<_m>(not_minus0(static_cast<double>(y)))), _z(static_cast<_m>(not_minus0(static_cast<double>(z)))) {}

// //     //! @brief 三次元のデータを作成する
// //     //! @details -0.0は0.0に変換して扱います
// //     //! @param r 大きさ
// //     //! @param theta 極角θ．+z軸を0として，-z軸に向かう方向が正です  0 ~ π
// //     //! @param phi 方位角φ．+x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π
// //     Position3D(_m r, Theta theta, Phi phi):
// //         _x(static_cast<double>(r)*sin(double(_rad(theta)))*cos(double(_rad(phi)))), _y(double(r)*sin(double(_rad(theta)))*sin(double(_rad(phi)))), _z(double(r)*cos(double(_rad(theta)))) {}

//     // //! @brief 3次元データのx成分を返します
//     // _m x() const noexcept 
//     //     {return _x;}

//     // //! @brief 3次元データのy成分を返します
//     // _m y() const noexcept 
//     //     {return _y;}

//     // //! @brief 3次元データのz成分を返します
//     // _m z() const noexcept 
//     //     {return _z;}

//     // //! @brief 3次元データの大きさ，原点からの距離を返します
//     // _m r() const
//     //     {return static_cast<_m>(std::sqrt(static_cast<double>(_x)*static_cast<double>(_x) + static_cast<double>(_y)*static_cast<double>(_y) + static_cast<double>(_z)*static_cast<double>(_z)));}

//     // //! @brief 3次元データの極角θを返します
//     // //! @note +z軸を0として，-z軸に向かう方向が正です  0 ~ π
//     // Theta theta() const
//     //     {return static_cast<Theta>(static_cast<_rad>(atan(std::sqrt(static_cast<double>(_x)*static_cast<double>(_x) + static_cast<double>(_y)*static_cast<double>(_y)) / static_cast<double>(_z)) + (static_cast<double>(_z)>0.0 ? 0.0 : 2.0*M_PI)));}

//     // //! @brief 3次元データの方位角φを返します．
//     // //! @note +x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π
//     // Phi phi() const
//     //     {return static_cast<Phi>(static_cast<_rad>(atan(static_cast<double>(_y)/static_cast<double>(_x)) + (static_cast<double>(_x)<0.0 ? M_PI : (static_cast<double>(_y)<0 ? 2.0*M_PI : 0.0))));}

// //     virtual operator Position<World>() = 0;
// // };

// //! @brief 地心座標系での位置
// //! @note 原点:地球の重心，+x:経度0°，+y:東経90°，+z:地軸(北)
// template<>
// class Position<World>
// {
//     const _m _x;
//     const _m _y;
//     const _m _z;

// public:
//     //! @brief 地心座標系での位置データを作成する
//     //! @details -0.0は0.0に変換して扱います
//     Position(_m x, _m y, _m z):
//         _x(static_cast<_m>(not_minus0(static_cast<double>(x)))), _y(static_cast<_m>(not_minus0(static_cast<double>(y)))), _z(static_cast<_m>(not_minus0(static_cast<double>(z)))) {}

//     //! @brief 地心座標系での位置データを作成する
//     //! @details -0.0は0.0に変換して扱います
//     //! @param r 大きさ
//     //! @param theta 極角θ．+z軸を0として，-z軸に向かう方向が正です  0 ~ π
//     //! @param phi 方位角φ．+x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π
//     Position(_m r, Theta theta, Phi phi):
//         _x(static_cast<double>(r)*sin(double(_rad(theta)))*cos(double(_rad(phi)))), _y(double(r)*sin(double(_rad(theta)))*sin(double(_rad(phi)))), _z(double(r)*cos(double(_rad(theta)))) {}

//     //! @brief 位置 (緯度，経度, [標高])
//     Position(Latitude lat, Longitude lon, Altitude alt = 0_m):
//         Position(
//             _m((Earth_N(lat)+double(alt.to_HAE())) * cos(double(_rad(lat))) * cos(double(_rad(lon)))), 
//             _m((Earth_N(lat)+double(alt.to_HAE())) * cos(double(_rad(lat))) * sin(double(_rad(lon)))),
//             _m((Earth_N(lat)*(1.0-Earth_e*Earth_e) + double(alt.to_HAE())) * sin(double(_rad(lat))))
//         ) {}
//     // 上記の計算式の作成に当たり，以下の資料を参考にしました
//     // https://psgsv.gsi.go.jp/koukyou/jyunsoku/pdf/r2/r2_shinkyu_furoku6.pdf

//     //! @brief 緯度を取得
//     Latitude latitude() const
//         {return static_cast<Latitude>(theta());}

//     //! @brief 経度を取得
//     Longitude longitude() const
//         {return static_cast<Longitude>(phi());}

//     //! @brief 標高を取得
//     Altitude altitude() const
//         {return static_cast<_m>((std::sqrt(double(x())*double(x()) + double(y())*double(y())) / cos(double(_rad(Latitude(theta())))) ) - Earth_N(Latitude(theta())));}
//     // この関数を作成するにあたり，以下の資料を参考にしました
//     // https://vldb.gsi.go.jp/sokuchi/surveycalc/surveycalc/algorithm/trans/trans_alg.html

//     //! @brief 3次元データのx成分を返します
//     _m x() const noexcept 
//         {return _x;}

//     //! @brief 3次元データのy成分を返します
//     _m y() const noexcept 
//         {return _y;}

//     //! @brief 3次元データのz成分を返します
//     _m z() const noexcept 
//         {return _z;}

//     //! @brief 3次元データの大きさ，原点からの距離を返します
//     _m r() const
//         {return static_cast<_m>(std::sqrt(static_cast<double>(_x)*static_cast<double>(_x) + static_cast<double>(_y)*static_cast<double>(_y) + static_cast<double>(_z)*static_cast<double>(_z)));}

//     //! @brief 3次元データの極角θを返します
//     //! @note +z軸を0として，-z軸に向かう方向が正です  0 ~ π
//     Theta theta() const
//         {return static_cast<Theta>(static_cast<_rad>(atan(std::sqrt(static_cast<double>(_x)*static_cast<double>(_x) + static_cast<double>(_y)*static_cast<double>(_y)) / static_cast<double>(_z)) + (static_cast<double>(_z)>0.0 ? 0.0 : 2.0*M_PI)));}

//     //! @brief 3次元データの方位角φを返します．
//     //! @note +x軸を0として，x軸からy軸に向かう方向が正です  0 ~ 2π
//     Phi phi() const
//         {return static_cast<Phi>(static_cast<_rad>(atan(static_cast<double>(_y)/static_cast<double>(_x)) + (static_cast<double>(_x)<0.0 ? M_PI : (static_cast<double>(_y)<0 ? 2.0*M_PI : 0.0))));}
// };

// //! @brief 局所座標系での位置
// //! @note 原点:地上の基準点，+x:南，+y:東，+z:天頂
// template<>
// class Position<Local> : private Position<World>
// {
// public:
//     static inline Position<World> BaseOrigin{Latitude(35.861268_deg), Longitude(139.607155_deg), Altitude(5_m)};  // 局所座標系の原点 (外部から変更可)

//     Position():
//         Position<World>() {}

// };

// //! @brief 機体座標系での位置
// //! @note 原点:機体の重心，+x:前，+y:左，+z:上
// template<>
// class Position<Body> : public Position3D
// {
// public:
//     using Position3D::Position3D;

//     static inline Position<World> BasePosition{Latitude(35.861268_deg), Longitude(139.607155_deg), Altitude(5_m)};  // 機体座標系の原点 (外部から変更可)
//     static inline Posture<Local> BasePosture{0_rad, 0_rad, 0_rad};  // 機体座標系の傾き

// };


// class Temperature;  // 気温
// class Pressure;  // 気圧
// class Humidity;  // 湿度
// class Acceleration;  // 加速度
// class Gyro;  // 角速度
// class magnetism;  // 磁気


// }

// #endif  // SC19_PICO_SC_MEASUREMENT_HPP_