#ifndef SC19_PICO_SC_PWM_HPP_
#define SC19_PICO_SC_PWM_HPP_

/**************************************************
 * PWMに関するコードです
 * このファイルは，pwm.cppに書かれている関数の一覧です
 * 
 * このファイルでは，PWMによる出力に関するコードが宣言されています．
**************************************************/

//! @file pwm.hpp
//! @brief PWMによる出力

#include "sc_basic.hpp"

#include "gpio.hpp"

namespace sc 
{

// PWMのDuty比 (ピンの出力が1になっている割合)
class Duty 
{
    static constexpr float MaxDuty = 1.0;  // Duty比の最大値
    static constexpr float MinDuty = 0.0;  // Duty比の最小値
    const float _duty;  // Duty比
public:
    //! @brief Duty比を指定
    //! @param duty Duty比 (ピンの出力が1になっている割合)
    Duty(float duty);

    constexpr operator float() const noexcept
        {return _duty;}
};

// PWM(パルス幅変調)を行います
class PWM
{
public:
    //! @brief PWMのセットアップ
    //! @param pin PWMで出力するためのピン
    //! @param freq 設定する周波数 (実際に設定される周波数は少しずれる)
    PWM(Pin pin, Frequency<Unit::Hz> freq = 0xffff_hz);

private:
    static constexpr float SysClock = 125E6;  // PWMの基準として使うシステムクロックの周波数 (Hz)
    enum class Channel  // チャンネル (1つのスライスに2つのピンが対応しているため，そのどちらを指すかを識別する値)
    {
        A,
        B
    };

    const Pin _pin;
    const uint8_t _slice;  // スライス (PWMにおいてピンを識別する番号)
    const Channel _channel;  // チャンネル (1つのスライスに2つのピンが対応しているため，そのどちらを指すかを識別する値)
    const uint16_t _wrap;  // 分解能 (詳しくは下記の資料へ)
    const float _clk_div;  // 分周比 (詳しくは下記の資料へ)
    static inline const Frequency<Unit::Hz> MaxFreq = 125000000_hz;  // PWMで扱える周波数の最大値
    static inline const Frequency<Unit::Hz> MinFreq = 8.0_hz;  // PWMで扱える周波数の最小値
    static constexpr uint16_t MaxWrap = 0xffff;  // PWMで扱えるWrapの最大値
    static constexpr uint16_t MinWrap = 1;  // PWMで扱えるWrapの最小値

public:
    //! @brief ピンの出力レベル(ピンの出力が1になっている割合)を設定
    //! @param duty : 出力レベル (0.0 ~ 1.0)
    void write(Duty duty) const;

    //! @brief 1周期のうちで出力がHighになっている時間を設定
    //! @param high_time high(1)になる時間
    void write(Time<Unit::s> high_time) const;

private:
    //! @brief 周波数から，設定できる最大の分解能(カウンタの最大値)を計算  (詳しくは下記の資料へ)
    //! @param freq 設定したい周波数
    static uint16_t to_wrap(Frequency<Unit::Hz> freq);

    //! @brief 周波数と分解能から分周比を計算  (詳しくは下記の資料へ)
    //! @param freq 設定したい周波数
    //! @param wrap 設定したい分解能
    static float to_clk_div(Frequency<Unit::Hz> freq, uint16_t wrap);

};
// PWMクラスは以下の資料を参考にして作成しました
// https://lipoyang.hatenablog.com/entry/2021/12/12/201432
// https://rikei-tawamure.com/entry/2021/02/08/213335


}

#endif  // SC19_PICO_SC_PWM_HPP_