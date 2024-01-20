#ifndef SC19_PICO_SC_PIN_HPP_
#define SC19_PICO_SC_PIN_HPP_

/**************************************************
 * ピンの使用に関するコードです
 * このファイルは，pin.cppに書かれている関数の一覧です
 * 
 * このファイルでは，ピンのGPIO番号などに関するコードが宣言されています．
**************************************************/

//! @file pin.hpp
//! @brief ピンの情報

// #include "sc_basic.hpp"

#include <vector>


namespace sc
{
    
//! @brief ピンの種類
enum class PinStatus
{
    NoUse,
    Gpio,
    I2cSda,
    I2cScl,
    SpiMiso,
    SpiCs,
    SpiSck,
    SpiMosi
};

//! @brief ピンのプルアップ・プルダウン
//! @note Pinクラスで使用します
enum class Pull
{
    No,  // プルアップ・ダウンを使用しない
    Up,  // プルアップ
    Down  //　プルダウン
};

//! @brief ピンのGPIO番号
class Pin
{
    const uint8_t _pin_gpio;  //! 扱うピンのGPIO番号
    
    static constexpr uint8_t MinGpio = 0;   //! 最小のGPIOピン番号
    static constexpr uint8_t MaxGpio = 28;  //! 最大のGPIOピン番号
public:
    //! @brief ピンのGPIO番号
    //! @param pin_gpio GPIO番号
    Pin(int pin_gpio);

    //! @brief GPIO番号を取得
    //! @return GPIO番号
    uint8_t get_gpio() const;
    
    //! @brief Pinをuint8_t型に変換
    //! @return uint8_t型のGPIO番号
    operator uint8_t() const;

    //! @brief プルアップ/プルダウンを設定
    //! @param pin プル抵抗の設定を行う対象のGPIO番号
    //! @param pull プルアップ/プルダウンを指定
    void set_pull(Pull pull) const; 

    //! @brief 現在のピンの使用状況．要素の番号はGPIOピン番号を表す
    static std::vector<PinStatus> Status;
};

}

#endif  // SC19_PICO_SC_PIN_HPP_