#ifndef SC19_PICO_SC_UART_HPP_
#define SC19_PICO_SC_UART_HPP_

/**************************************************
 * UART通信の出入力に関するコードです
 * このファイルは，uart.cppに書かれている関数の一覧です
 * 
 * このファイルでは，UART通信に関するコードが宣言されています．
**************************************************/

//! @file uart.hpp
//! @brief UARTでの入出力

#include <deque>
#include <vector>

#include "sc_basic.hpp"
#include "binary.hpp"

#include "hardware/uart.h"


namespace sc
{

//! @brief UART0かUART1か
enum UART_ID
{
    uart_0,
    uart_1
};

//! @brief  UARTのTXピン
class TX : public Pin
{
    static constexpr uint8_t EnableUART0_TX[] = {0, 12, 16};  // UART0のTXピンのGPIO番号が取り得る値
    static constexpr uint8_t EnableUART1_TX[] = {4, 8};  // UART1のTXピンのGPIO番号が取り得る値
public:
    //! @brief TXピンを指定
    //! @param tx_gpio TXピンのGPIO番号
    TX(int tx_gpio);

    //! @brief uartポートの種類を返す
    //! @return uart0かuart1か
    UART_ID get_uart_id() const;
};

//! @brief  UARTのRXピン
class RX : public Pin
{
    static constexpr uint8_t EnableUART0_RX[] = {1, 13, 17};  // UART0のRXピンのGPIO番号が取り得る値
    static constexpr uint8_t EnableUART1_RX[] = {5, 9};  // UART1のRXピンのGPIO番号が取り得る値
public:
    //! @brief RXピンを指定
    //! @param rx_gpio RXピンのGPIO番号
    RX(int rx_gpio);

    //! @brief uartポートの種類を返す
    //! @return uart0かuart1か
    UART_ID get_uart_id() const;
};


//! @brief UART通信
class UART : Noncopyable
{
private:
    const TX _tx;  // UARTで使用するTXピン
    const RX _rx;  // UARTで使用するRXピン
    const Frequency<Unit::Hz>  _freq;  // UARTの通信速度
    const UART_ID _uart_id;  // UART0かUART1か

public:
    //! @brief UARTをセットアップ
    //! @param tx UARTで使用するTXピン
    //! @param rx UARTで使用するRXピン
    //! @param freq UARTの通信速度 (10000_hz のように入力)
    UART(TX tx, RX rx, Frequency<Unit::Hz>  freq);

    //! @brief UARTによる送信
    //! @param output_data 送信するデータ
    void write(Binary output_data) const;

    //! @brief UARTによる受信
    //! @return Binary型のバイト列
    Binary read() const;

private:
    static inline bool IsUse[2] = {false, false};  // 既にUART0とUART1を使用しているか
    static constexpr std::size_t MaxInputLen = 100;  // 受信したデータを最大で何バイトまで保管しておくか

public:
    static inline std::deque<uint8_t> uart0_queue{MaxInputLen, 0};
    static inline std::deque<uint8_t> uart1_queue{MaxInputLen, 0};
    static void uart0_handler();
    static void uart1_handler();
};

}

#endif  // SC19_PICO_SC_UART_HPP_