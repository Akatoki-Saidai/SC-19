#ifndef SC19_PICO_SC_UNIT_HPP_
#define SC19_PICO_SC_UNIT_HPP_

/**************************************************
 * 単位に関するコードです
 * このファイルは，unit.cppに書かれている関数の一覧です
 * 
 * このファイルでは，単位の利用や変換に関するコードが宣言されています．
**************************************************/

//! @file unit.hpp
//! @brief 単位のクラスと変換

// #include "sc_basic.hpp"

#include <cmath>


namespace sc
{

static constexpr double PI = 3.14159265358979323846;  // 円周率π

// べき乗の計算の内部処理用
template<int N, class Now, class Start>
static constexpr inline auto pow_calc(Now now, Start start)
{
    if constexpr (N==0)
        {return now;}
    else if constexpr (N>0)
        {return pow_calc<N-1>(now*start, start);}
    else
        {return pow_calc<N+1>(now/start, start);}
}

//! @brief べき乗を計算
//! @param num べき乗したい数
//! @note pow<2>(3.4);  で3.4の2乗を計算する
template<int N, class T>
constexpr inline auto pow(T num)
{
    return pow_calc<N>(num/num, num);
}


/***** 単位と次元 *****/


// SI単位の定義
// 秒（記号は s）は、時間の SI 単位であり、セシウム周波数 ∆νCs、すなわち、セシウム 133 原子の摂動を受けない基底状態の超微細構造遷移周波数を単位Hz（s−1 に等しい）で表したときに、その数値を 9 192 631 770 と定めることによって定義される。
// メートル（記号は m）は長さの SI 単位であり、真空中の光の速さ c を単位m s−1 で表したときに、その数値を 299 792 458 と定めることによって定義される。ここで、秒はセシウム周波数 ∆νCs によって定義される。
// キログラム（記号は kg）は質量の SI 単位であり、プランク定数 h を単位 J s（kg m2 s−1 に等しい）で表したときに、その数値を 6.626 070 15 × 10−34 と定めることによって定義される。ここで、メートルおよび秒は c および ∆νCs に関連して定義される。
// アンペア（記号は A）は、電流の SI 単位であり、電気素量 e を単位 C（A s に等しい）で表したときに、その数値を 1.602 176 634 × 10−19 と定めることによって定義される。ここで、秒は ∆νCs によって定義される。
// ケ ル ビ ン（ 記 号 は K） は、 熱 力 学 温 度 の SI 単 位 で あ り、 ボ ル ツ マ ン 定数 k を 単 位 J K−1（kg m2 s−2 K−1 に 等 し い ） で 表 し た と き に、 そ の 数 値 を1.380 649 × 10−23 と定めることによって定義される。ここで、キログラム、メートルおよび秒は h、c および ∆νCs に関連して定義される。
// モ ル（ 記 号 は mol） は、 物 質 量 の SI 単 位 で あ り、1 モ ル に は、 厳 密 に6.022 140 76 × 1023 の要素粒子が含まれる。この数は、アボガドロ定数 NA を単位 mol−1 で表したときの数値であり、アボガドロ数と呼ばれる。
// 系の物質量（記号は n）は、特定された要素粒子の数の尺度である。要素粒子は、原子、分子、イオン、電子、その他の粒子、あるいは、粒子の集合体のいずれであってもよい。
// カンデラ（記号は cd）は、所定の方向における光度の SI 単位であり、周波数 540 × 10 12 Hz の単色放射の視感効果度 Kcd を単位 lm W−1（cd sr W−1 あるいは cd sr kg−1 m−2 s3 に等しい）で表したときに、その数値を 683 と定めることによって定義される。ここで、キログラム、メートルおよび秒は h、c および∆νCs に関連して定義される。
// https://unit.aist.go.jp/nmij/public/report/si-brochure/pdf/SI_9th_%E6%97%A5%E6%9C%AC%E8%AA%9E%E7%89%88_r.pdf より引用

//! @brief 物理量の次元を表すクラス
//! @param T 時間(s)
//! @param L 長さ(m)
//! @param M 質量(kg)
//! @param I 電流(A)
//! @param Th 温度(K)
//! @param N 物質量(mol)
//! @param J 光度(cd)
template<int T, int L, int M, int I, int Th, int N, int J>
class Dimension 
{
    const double _number;  // 数値
    
public:
    //! @brief 次元を持った量を，同じ次元型から構築
    constexpr Dimension(const Dimension<T, L, M, I, Th, N, J>& dim) = default;

    //! @brief 次元を持った，単位量を構築
    constexpr Dimension():
        _number(1.0) {}

    //! @brief 次元を持った量を構築
    explicit constexpr Dimension(const double& num):
        _number(num) {}
    
    explicit constexpr operator double() const
        {return _number;}

    constexpr double number() const
        {return _number;}    
};

// 定義定数
namespace constant 
{
    constexpr Dimension<-1, 0, 0, 0, 0, 0, 0> dNuCs{9'192'631'770.0};  // Δνcs．セシウム133原子の摂動を受けない基底状態の超微細構造遷移周波数
    constexpr Dimension<-1, 1, 0, 0, 0, 0, 0> c{299'792'458.0};  // c．真空中の光の速さ
    constexpr Dimension<-1, 2, 1, 0, 0, 0, 0> h{6.626'070'15E-34};  // h．プランク定数
    constexpr Dimension<1, 0, 0, 1, 0, 0, 0> e{1.602'176'634E-19};  // e．電気素量
    constexpr Dimension<-2, 2, 1, 0, -1, 0, 0> k{1.380'649E-23};  // k．ボルツマン定数   
    constexpr Dimension<0, 0, 0, 0, 0, -1, 0> NA{6.022'140'76E23};  // NA．アボガドロ定数
}


// 演算子

template<int T, int L, int M, int I, int Th, int N, int J>
Dimension<T, L, M, I, Th, N, J> constexpr operator+(const Dimension<T, L, M, I, Th, N, J>& scalar)
{
    return scalar;
}

template<int T, int L, int M, int I, int Th, int N, int J>
Dimension<T, L, M, I, Th, N, J> constexpr operator-(const Dimension<T, L, M, I, Th, N, J>& scalar)
{
    return Dimension<T, L, M, I, Th, N, J>{-static_cast<double>(scalar)};
}

template<int T, int L, int M, int I, int Th, int N, int J>
Dimension<T, L, M, I, Th, N, J> constexpr operator+(const Dimension<T, L, M, I, Th, N, J>& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return Dimension<T, L, M, I, Th, N, J>{static_cast<double>(scalar1) + static_cast<double>(scalar2)};
}

template<int T, int L, int M, int I, int Th, int N, int J>
Dimension<T, L, M, I, Th, N, J> constexpr operator-(const Dimension<T, L, M, I, Th, N, J>& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return Dimension<T, L, M, I, Th, N, J>{static_cast<double>(scalar1) - static_cast<double>(scalar2)};
}

template<int T, int L, int M, int I, int Th, int N, int J>
Dimension<T, L, M, I, Th, N, J> constexpr operator*(const Dimension<T, L, M, I, Th, N, J>& scalar1, const double& scalar2)
{
    return Dimension<T, L, M, I, Th, N, J>{static_cast<double>(scalar1) * scalar2};
}

template<int T, int L, int M, int I, int Th, int N, int J>
Dimension<T, L, M, I, Th, N, J> constexpr operator*(const double& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return Dimension<T, L, M, I, Th, N, J>{scalar1 * static_cast<double>(scalar2)};
}

template<int T1, int L1, int M1, int I1, int Th1, int N1, int J1, int T2, int L2, int M2, int I2, int Th2, int N2, int J2>
Dimension<T1+T2, L1+L2, M1+M2, I1+I2, Th1+Th2, N1+N2, J1+J2> constexpr operator*(const Dimension<T1, L1, M1, I1, Th1, N1, J1>& scalar1, const Dimension<T2, L2, M2, I2, Th2, N2, J2>& scalar2)
{
    return Dimension<T1+T2, L1+L2, M1+M2, I1+I2, Th1+Th2, N1+N2, J1+J2>{static_cast<double>(scalar1) * static_cast<double>(scalar2)};
}

template<int T, int L, int M, int I, int Th, int N, int J>
Dimension<T, L, M, I, Th, N, J> constexpr operator/(const Dimension<T, L, M, I, Th, N, J>& scalar1, const double& scalar2)
{
    return Dimension<T, L, M, I, Th, N, J>{static_cast<double>(scalar1) / scalar2};
}

template<int T, int L, int M, int I, int Th, int N, int J>
Dimension<-T, -L, -M, -I, -Th, -N, -J> constexpr operator/(const double& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return Dimension<-T, -L, -M, -I, -Th, -N, -J>{scalar1 / static_cast<double>(scalar2)};
}

template<int T1, int L1, int M1, int I1, int Th1, int N1, int J1, int T2, int L2, int M2, int I2, int Th2, int N2, int J2>
Dimension<T1-T2, L1-L2, M1-M2, I1-I2, Th1-Th2, N1-N2, J1-J2> constexpr operator/(const Dimension<T1, L1, M1, I1, Th1, N1, J1>& scalar1, const Dimension<T2, L2, M2, I2, Th2, N2, J2>& scalar2)
{
    return Dimension<T1-T2, L1-L2, M1-M2, I1-I2, Th1-Th2, N1-N2, J1-J2>{static_cast<double>(scalar1) / static_cast<double>(scalar2)};
}

template<int T, int L, int M, int I, int Th, int N, int J>
bool constexpr operator==(const Dimension<T, L, M, I, Th, N, J>& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return (static_cast<double>(scalar1) == static_cast<double>(scalar2));
}

template<int T, int L, int M, int I, int Th, int N, int J>
bool constexpr operator!=(const Dimension<T, L, M, I, Th, N, J>& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return (static_cast<double>(scalar1) != static_cast<double>(scalar2));
}

template<int T, int L, int M, int I, int Th, int N, int J>
bool constexpr operator<(const Dimension<T, L, M, I, Th, N, J>& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return (static_cast<double>(scalar1) < static_cast<double>(scalar2));
}

template<int T, int L, int M, int I, int Th, int N, int J>
bool constexpr operator<=(const Dimension<T, L, M, I, Th, N, J>& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return (static_cast<double>(scalar1) <= static_cast<double>(scalar2));
}

template<int T, int L, int M, int I, int Th, int N, int J>
bool constexpr operator>(const Dimension<T, L, M, I, Th, N, J>& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return (static_cast<double>(scalar1) > static_cast<double>(scalar2));
}

template<int T, int L, int M, int I, int Th, int N, int J>
bool constexpr operator>=(const Dimension<T, L, M, I, Th, N, J>& scalar1, const Dimension<T, L, M, I, Th, N, J>& scalar2)
{
    return (static_cast<double>(scalar1) >= static_cast<double>(scalar2));
}


// 単位
enum class Unit 
{
    // SI単位
    s, m, kg, A, K, mol, cd,

    // 固有の名称と記号を持つSI単位
    rad, sr, Hz, N, Pa, J, W, C, V, F, Ohm, S, Wb, T, H, degC, lm, lx, Bq, Gy, Sv, kat,

    // その他のSI組立単位
    m_s, m_s2, rad_s,

    // 非SI単位
    deg, h, min, eV, px, percent
};

// 単位の次元
namespace dimension
{
    // 無次元量
    using pure = Dimension<0, 0, 0, 0, 0, 0, 0>;
    
    // SI基本単位の次元
    using s = Dimension<1, 0, 0, 0, 0, 0, 0>;
    using m = Dimension<0, 1, 0, 0, 0, 0, 0>;
    using kg = Dimension<0, 0, 1, 0, 0, 0, 0>;
    using A = Dimension<0, 0, 0, 1, 0, 0 ,0>;
    using K = Dimension<0, 0, 0, 0, 1, 0, 0>;
    using mol = Dimension<0, 0, 0, 0, 0, 1, 0>;
    using cd = Dimension<0, 0, 0, 0, 0, 0, 1>;

    // 固有の名称と記号を持つSI単位の次元
    using rad = decltype(m{} / m{});
    using sr = decltype(m{}*m{} / (m{}*m{}));
    using Hz = decltype(1 / s{});
    using N = decltype(kg{} * m{} / (s{}*s{}));
    using Pa = decltype(kg{} / (m{} * s{}*s{}));
    using J = decltype(kg{} * m{}*m{} / (s{}*s{}));
    using W = decltype(kg{} * m{}*m{} / (s{}*s{}*s{}));
    using C = decltype(A{} * s{});
    using V = decltype(kg{} * m{}*m{} / (s{}*s{}*s{} * A{}));
    using F = decltype(s{}*s{}*s{}*s{} * A{}*A{} / (kg{} * m{}*m{}));
    using Ohm = decltype(kg{} * m{}*m{} / (s{}*s{}*s{} * A{}*A{}));
    using S = decltype(s{}*s{}*s{} * A{}*A{} / (kg{} * m{}*m{}));
    using Wb = decltype(kg{} * m{}*m{} / (s{}*s{} * A{}));
    using T = decltype(kg{} / (s{}*s{} *A{}));
    using H = decltype(kg{} * m{}*m{} / (s{}*s{} * A{}*A{}));
    using degC = K;
    using lm = decltype(cd{} * sr{});
    using lx = decltype(cd{} * sr{} / (m{}*m{}));
    using Bq = decltype(1 / s{});
    using Gy = decltype(m{}*m{} / (s{}*s{}));
    using Sv = decltype(m{}*m{} / (s{}*s{}));
    using kat = decltype(mol{} / s{});

    // その他のSI組立単位の次元
    using m_s = decltype(m{} / s{});
    using m_s2 = decltype(m{} / (s{}*s{}));
    using rad_s = decltype(rad{} / s{});

    // 非SI単位の次元
    using deg = rad;
    using h = s;
    using min = s;
    using eV = J;
    using px = pure;
    using percent = pure;
}
// 以下の資料を参考にしました
// https://unit.aist.go.jp/nmij/public/report/si-brochure/pdf/SI_9th_%E6%97%A5%E6%9C%AC%E8%AA%9E%E7%89%88_r.pdf


/***** SI接頭語 *****/

constexpr double Quetta = 1E30;
constexpr double Ronna = 1E27;
constexpr double Yotta = 1E24;
constexpr double Zetta = 1E21;
constexpr double Exa = 1E18;
constexpr double Peta = 1E15;
constexpr double Tera = 1E12;
constexpr double Giga = 1E9;
constexpr double Mega = 1E6;
constexpr double kiro = 1E3;
constexpr double hecto = 1E2;
constexpr double deca = 1E1;
constexpr double deci = 1E-1;
constexpr double centi = 1E-2;
constexpr double milli = 1E-3;
constexpr double micro = 1E-6;
constexpr double nano = 1E-9;
constexpr double pico = 1E-12;
constexpr double femto = 1E-15;
constexpr double atto = 1E-18;
constexpr double zepto = 1E-21;
constexpr double yocto = 1E-24;
constexpr double ronto = 1E-27;
constexpr double quecto = 1E-30;


/***** ベクトル量 *****/

//! @brief ベクトル量
template<class Element>
class Vector3 
{
    const Element _data[3];

public:
    //! @brief 3つの値からベクトルを構築
    constexpr Vector3(Element x, Element y, Element z):
        _data({x, y, z}) {}

    constexpr Element x() const
        {return _data[0];}
    constexpr Element y() const
        {return _data[1];}
    constexpr Element z() const
        {return _data[2];}

    Element constexpr operator[] (std::size_t index) const
        {return _data[index];}
};

//! @brief ベクトルの単項+
template<class Element>
constexpr auto operator+ (const Vector3<Element>& vector)
{
    return Vector3(+vector[0], +vector[1], +vector[2]);
}

//! @brief ベクトルの単項-
template<class Element>
constexpr auto operator- (const Vector3<Element>& vector)
{
    return Vector3(-vector[0], -vector[1], -vector[2]);
}

//! @brief ベクトルのスカラー倍
template<class Element, class Scalar>
constexpr auto operator* (const Vector3<Element>& vector3, const Scalar& scalar) -> Vector3<decltype(vector3[0]*scalar)>
{
    return Vector3(vector3[0]*scalar, vector3[1]*scalar, vector3[2]*scalar);
}

//! @brief ベクトルのスカラー倍
template<class Scalar, class Element>
constexpr auto operator* (const Scalar& scalar, const Vector3<Element>& vector3) -> Vector3<decltype(scalar*vector3[0])>
{
    return Vector3(scalar*vector3[0], scalar*vector3[1], scalar*vector3[2]);
}

//! @brief ベクトルの足し算
template<class Element1, class Element2>
constexpr auto operator+ (Vector3<Element1> vector1, Vector3<Element2> vector2) -> Vector3<decltype(vector1[0]+vector2[0])>
{
    return Vector3(vector1[0]+vector2[0], vector1[1]+vector2[1], vector1[2]+vector2[2]);
}

//! @brief ベクトルの引き算
template<class Element1, class Element2>
constexpr auto operator- (Vector3<Element1> vector1, Vector3<Element2> vector2) -> Vector3<decltype(vector1[0]-vector2[0])>
{
    return Vector3(vector1[0]-vector2[0], vector1[1]-vector2[1], vector1[2]-vector2[2]);
}

//! @brief ベクトルの内積
template<class Element1, class Element2>
constexpr auto operator* (Vector3<Element1> vector1, Vector3<Element2> vector2) -> decltype(vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2])
{
    return vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2];
}

//! @brief ベクトルの外積
template<class Element1, class Element2>
constexpr auto operator% (Vector3<Element1> vector1, Vector3<Element2> vector2) -> Vector3<decltype(vector1[1]*vector2[2] - vector1[2]*vector2[1])>
{
    return Vector3(vector1[1]*vector2[2] - vector1[2]*vector2[1], vector1[2]*vector2[0] - vector1[0]*vector2[2], vector1[0]*vector2[1] - vector1[1]*vector2[0]);
}


/***** 物理量 *****/

template<Unit> class Length;  // 長さ
template<class T> Length(const T& t) -> Length<Unit::m>;  // デフォルトをmにする

//! @brief 長さ(m)
template<>
class Length<Unit::m> : public dimension::m 
{
public:
    //! @brief 長さ(m)
    explicit constexpr Length(const double& num):
        dimension::m(num) {}

    //! @brief 長さ(m)
    constexpr Length(const dimension::m& num):
        dimension::m(num) {}

    //! @brief 長さをdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};


template<Unit> class Angle;  // 角度
template<class T> Angle(const T& t) -> Angle<Unit::rad>;  // デフォルトをradにする

//! @brief 角度(rad)
template<>
class Angle<Unit::rad> : public dimension::rad
{
public:
    //! @brief 角度(rad)
    explicit constexpr Angle(const double& num):
        dimension::rad(num) {}

    //! @brief 角度(rad)
    constexpr Angle(const dimension::rad& num):
        dimension::rad(num) {}

    //! @brief 角度(rad)をdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};

// 角度(deg)
template<>
class Angle<Unit::deg> : public Angle<Unit::rad> 
{
    //! @brief 角度(deg)を角度(rad)に変換
    static constexpr double deg_to_rad(const double& num)
        {return num * PI / 180.0;}

    //! @brief 角度(rad)を角度(deg)に変換
    static constexpr double rad_to_deg(const double& num)
        {return num * 180.0 / PI;}
public:
    //! @brief 角度(deg)
    explicit constexpr Angle(const double& num):
        Angle<Unit::rad>(deg_to_rad(num)) {}

    //! @brief 角度(deg)
    constexpr Angle(const Angle<Unit::rad>& time):
        Angle<Unit::rad>(time) {}
    
    //! @brief 角度(rad)をdoubleに変換
    explicit constexpr operator double() const
        {return rad_to_deg(number());}
};


template<Unit> class Time;  // 時間
template<class T> Time(const T& t) -> Time<Unit::s>;  // デフォルトを秒にする

// 秒(s)
template<>
class Time<Unit::s> : public dimension::s
{
public:
    explicit constexpr Time(const double& num):
        dimension::s(num) {}

    constexpr Time(const dimension::s& num):
        dimension::s(num) {}

    explicit constexpr operator double() const
        {return number();}
};


template<Unit> class Frequency;  // 周波数
template<class T> Frequency(const T& t) -> Frequency<Unit::Hz>;  // デフォルトをHzにする

//! @brief 周波数(Hz)
template<>
class Frequency<Unit::Hz> : public dimension::Hz
{
public:
    //! @brief 周波数(Hz)
    explicit constexpr Frequency(const double& num):
        dimension::Hz(num) {}

    //! @brief 周波数(Hz)
    constexpr Frequency(const dimension::Hz& num):
        dimension::Hz(num) {}

    //! @brief 周波数をdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};


template<Unit> class Velocity;  // 速度
template<class T> Velocity(const T&, const T&, const T&) -> Velocity<Unit::m_s>;  // デフォルトをm_sにする

//! @brief 速度(m_s)
template<>
class Velocity<Unit::m_s> : public Vector3<dimension::m_s>
{
public:
    using Vector3<dimension::m_s>::Vector3;
};


template<Unit> class Acceleration;  // 加速度
template<class T> Acceleration(const T&, const T&, const T&) -> Acceleration<Unit::m_s2>;  // デフォルトをm_s2にする

//! @brief 加速度(m_s2)
template<>
class Acceleration<Unit::m_s2> : public Vector3<dimension::m_s2>
{
public:
    using Vector3<dimension::m_s2>::Vector3;
};


template<Unit> class Temperature;  // 温度
template<class T> Temperature(const T& t) -> Temperature<Unit::K>;  // デフォルトをKにする

//! @brief 温度(K)
template<>
class Temperature<Unit::K> : public dimension::K
{
public:
    //! @brief 温度(K)
    explicit constexpr Temperature(const double& num):
        dimension::K(num) {}

    //! @brief 温度(K)
    constexpr Temperature(const dimension::K& num):
        dimension::K(num) {}

    //! @brief 温度(K)をdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};

// 温度(degC)
template<>
class Temperature<Unit::degC> : public Temperature<Unit::K> 
{
    //! @brief 温度(℃)を温度(K)に変換
    static constexpr double degC_to_K(const double& num)
        {return num + 273.15;}

    //! @brief 温度(K)を温度(℃)に変換
    static constexpr double K_to_degC(const double& num)
        {return num - 273.15;}
public:
    //! @brief 温度(℃)
    explicit constexpr Temperature(const double& num):
        Temperature<Unit::K>(degC_to_K(num)) {}

    //! @brief 温度(℃)
    constexpr Temperature(const Temperature<Unit::K>& time):
        Temperature<Unit::K>(time) {}
    
    //! @brief 温度(℃)をdoubleに変換
    explicit constexpr operator double() const
        {return K_to_degC(number());}
};


template<Unit> class Pressure;  // 圧力
template<class T> Pressure(const T& t) -> Pressure<Unit::Pa>;  // デフォルトをPaにする

//! @brief 圧力(Pa)
template<>
class Pressure<Unit::Pa> : public dimension::Pa
{
public:
    //! @brief 圧力(Pa)
    explicit constexpr Pressure(const double& num):
        dimension::Pa(num) {}

    //! @brief 圧力(Pa)
    constexpr Pressure(const dimension::Pa& num):
        dimension::Pa(num) {}

    //! @brief 圧力(Pa)をdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};


template<Unit> class AngularVelocity;  // 角速度
template<class T> AngularVelocity(const T&, const T&, const T&) -> AngularVelocity<Unit::rad_s>;  // デフォルトをrad_sにする

//! @brief 角速度(rad_s)
template<>
class AngularVelocity<Unit::rad_s> : public Vector3<dimension::rad_s>
{
public:
    using Vector3<dimension::rad_s>::Vector3;
};


template<Unit> class MagneticFluxDensity;  // 磁束密度
template<class Type> MagneticFluxDensity(const Type&, const Type&, const Type&) -> MagneticFluxDensity<Unit::T>;  // デフォルトをTにする

//! @brief 磁束密度(T)
template<>
class MagneticFluxDensity<Unit::T> : public Vector3<dimension::T>
{
public:
    using Vector3<dimension::T>::Vector3;
};


template<Unit> class Illuminance;  // 照度
template<class T> Illuminance(const T& t) -> Illuminance<Unit::lx>;  // デフォルトをlxにする

//! @brief 照度(lx)
template<>
class Illuminance<Unit::lx> : public dimension::lx
{
public:
    //! @brief 照度(lx)
    explicit constexpr Illuminance(const double& num):
        dimension::lx(num) {}

    //! @brief 照度(lx)
    constexpr Illuminance(const dimension::lx& num):
        dimension::lx(num) {}

    //! @brief 照度(lx)をdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};


template<Unit> class DisplayResolution;  // 画素数
template<class T> DisplayResolution(const T& t) -> DisplayResolution<Unit::px>;  // デフォルトをpxにする

//! @brief 画素数(px)
template<>
class DisplayResolution<Unit::px> : public dimension::px
{
public:
    //! @brief 画素数(px)
    explicit constexpr DisplayResolution(const double& num):
        dimension::px(num) {}

    //! @brief 画素数(px)
    constexpr DisplayResolution(const dimension::px& num):
        dimension::px(num) {}

    //! @brief 画素数(px)をdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};


template<Unit> class Altitude;  // 標高
template<class T> Altitude(const T& t) -> Altitude<Unit::m>;  // デフォルトをmにする

//! @brief 標高(m)
template<>
class Altitude<Unit::m> : public dimension::m
{
    static double _pressure0;
    static double _temperature0;
    static double _altitude0;
public:
    //! @brief 標高(m)
    explicit constexpr Altitude(const double& num):
        dimension::m(num) {}

    //! @brief 標高(m)
    constexpr Altitude(const dimension::m& num):
        dimension::m(num) {}

    //! @brief 標高(m)をdoubleに変換
    explicit constexpr operator double() const
        {return number();}

    //! @brief
    static void set_origin(double pressure0=1013.25, double temperature0=20, double altitude0=0)
    {
        _pressure0    = pressure0;
        _temperature0 = temperature0;
        _altitude0    = altitude0;
    }

    Altitude(double pressure, double temperature):
        // dimension::m(_altitude0 + ((temperature + 273.15) / 0.0065F) * (std::pow((_pressure0 / pressure), 1.0F / 5.257F) -1.0F)) {}
        dimension::m(_altitude0 + ((_temperature0 + 273.15F) / 0.0065F) * (1 - std::pow((pressure / _pressure0), (1.0F / 5.257F)))) {}
};


template<Unit> class Latitude;  // 緯度
template<class T> Latitude(const T& t) -> Latitude<Unit::rad>;  // デフォルトをradにする

//! @brief 緯度(rad)
template<>
class Latitude<Unit::rad> : public dimension::rad
{
public:
    //! @brief 緯度(rad)
    explicit constexpr Latitude(const double& num):
        dimension::rad(num) {}

    //! @brief 緯度(rad)
    constexpr Latitude(const dimension::rad& num):
        dimension::rad(num) {}

    //! @brief 緯度(rad)をdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};

// 緯度(deg)
template<>
class Latitude<Unit::deg> : public Latitude<Unit::rad> 
{
    //! @brief 緯度(deg)を緯度(rad)に変換
    static constexpr double deg_to_rad(const double& num)
        {return num * PI / 180.0;}

    //! @brief 緯度(rad)を緯度(deg)に変換
    static constexpr double rad_to_deg(const double& num)
        {return num * 180.0 / PI;}
public:
    //! @brief 緯度(deg)
    explicit constexpr Latitude(const double& num):
        Latitude<Unit::rad>(deg_to_rad(num)) {}

    //! @brief 緯度(deg)
    constexpr Latitude(const Latitude<Unit::rad>& time):
        Latitude<Unit::rad>(time) {}
    
    //! @brief 緯度(rad)をdoubleに変換
    explicit constexpr operator double() const
        {return rad_to_deg(number());}
};


template<Unit> class Longitude;  // 経度
template<class T> Longitude(const T& t) -> Longitude<Unit::rad>;  // デフォルトをradにする

//! @brief 経度(rad)
template<>
class Longitude<Unit::rad> : public dimension::rad
{
public:
    //! @brief 経度(rad)
    explicit constexpr Longitude(const double& num):
        dimension::rad(num) {}

    //! @brief 経度(rad)
    constexpr Longitude(const dimension::rad& num):
        dimension::rad(num) {}

    //! @brief 経度(rad)をdoubleに変換
    explicit constexpr operator double() const
        {return number();}
};

// 経度(deg)
template<>
class Longitude<Unit::deg> : public Longitude<Unit::rad> 
{
    //! @brief 経度(deg)を経度(rad)に変換
    static constexpr double deg_to_rad(const double& num)
        {return num * PI / 180.0;}

    //! @brief 経度(rad)を経度(deg)に変換
    static constexpr double rad_to_deg(const double& num)
        {return num * 180.0 / PI;}
public:
    //! @brief 経度(deg)
    explicit constexpr Longitude(const double& num):
        Longitude<Unit::rad>(deg_to_rad(num)) {}

    //! @brief 経度(deg)
    constexpr Longitude(const Longitude<Unit::rad>& time):
        Longitude<Unit::rad>(time) {}
    
    //! @brief 経度(rad)をdoubleに変換
    explicit constexpr operator double() const
        {return rad_to_deg(number());}
};


template<Unit> class Humidity;  // 湿度
template<class T> Humidity(const T& t) -> Humidity<Unit::percent>;  // デフォルトをpercentにする

//! @brief 湿度(%)
template<>
class Humidity<Unit::percent> : public dimension::percent
{
public:
    //! @brief 湿度(%)
    explicit constexpr Humidity(const double& num):
        dimension::percent(num) {}

    //! @brief 湿度(%)
    constexpr Humidity(const dimension::percent& num):
        dimension::percent(num) {}

    //! @brief 湿度(%)をdoubleに変換
    //! @brief 50.0%なら，50.0が返される
    explicit constexpr operator double() const
        {return number();}
};

}


/***** ユーザー定義リテラル *****/

//! @brief 光度(cd)
inline sc::dimension::cd operator"" _cd(const char* cd_chars) {return sc::dimension::cd(std::stod(cd_chars, nullptr));}

//! @brief 温度(℃)
inline sc::dimension::degC operator"" _degC(const char* degC_chars) {return sc::dimension::degC(std::stod(degC_chars, nullptr));}

//! @brief 周波数(Hz)
inline sc::dimension::Hz operator"" _hz(const char* hz_chars) {return sc::dimension::Hz(std::stod(hz_chars, nullptr));}

//! @brief 長さ(m)
inline sc::dimension::m operator"" _m(const char* m_chars) {return sc::dimension::m(std::stod(m_chars, nullptr));}

//! @brief 長さ(km)
inline sc::dimension::m operator"" _km(const char* km_chars) {return sc::dimension::m(std::stod(km_chars, nullptr) * sc::kiro);}

//! @brief 加速度(m/s²)
inline sc::dimension::m_s2 operator"" _m_s2(const char* m_s2_chars) {return sc::dimension::m_s2(std::stod(m_s2_chars, nullptr));}

//! @brief 圧力(Pa)
inline sc::dimension::Pa operator"" _pa(const char* pa_chars) {return sc::dimension::Pa(std::stod(pa_chars, nullptr));}

//! @brief 圧力(hPa)
inline sc::dimension::Pa operator"" _hPa(const char* hPa_chars) {return sc::dimension::Pa(std::stod(hPa_chars, nullptr) * sc::hecto);}

//! @brief 画素数(px)
inline sc::dimension::px operator"" _px(const char* px_chars) {return sc::dimension::px(std::stod(px_chars, nullptr));}

//! @brief 角度(rad)
inline sc::dimension::rad operator"" _rad(const char* rad_chars) {return sc::dimension::rad(std::stod(rad_chars, nullptr));}

//! @brief 角度(rad)．数値×π (rad)
inline sc::dimension::rad operator"" _pi_rad(const char* rad_chars) {return sc::dimension::rad(sc::PI * std::stod(rad_chars, nullptr));}

//! @brief 角度(°)
inline sc::dimension::deg operator"" _deg(const char* deg_chars) {return sc::dimension::deg(std::stod(deg_chars, nullptr));}

//! @brief 角速度(rad/s)
inline sc::dimension::rad_s operator"" _rad_s(const char* rad_s_chars) {return sc::dimension::rad_s(std::stod(rad_s_chars, nullptr));}

// //! @brief 角速度(°/s)
// inline sc::dimension::deg_s operator"" _deg_s(const char* deg_s_chars) {return sc::dimension::deg_s(std::stod(deg_s_chars, nullptr));}

//! @brief マイクロ秒(μs)
inline sc::dimension::s operator"" _us(const char* us_chars) {return sc::dimension::s(std::stod(us_chars, nullptr) * sc::micro);}

//! @brief ミリ秒(ms)
inline sc::dimension::s operator"" _ms(const char* ms_chars) {return sc::dimension::s(std::stod(ms_chars, nullptr) * sc::milli);}

//! @brief 秒(s)
inline sc::dimension::s operator"" _s(const char* s_chars) {return sc::dimension::s(std::stod(s_chars, nullptr));}

//! @brief 分(min)
inline sc::dimension::s operator"" _min(const char* min_chars) {return sc::dimension::s(std::stod(min_chars, nullptr) * 60.0);}

//! @brief 磁束密度(mT)
inline sc::dimension::T operator"" _mT(const char* mT_chars) {return sc::dimension::T(std::stod(mT_chars, nullptr) * sc::milli);}



#endif  // SC19_PICO_SC_UNIT_HPP_