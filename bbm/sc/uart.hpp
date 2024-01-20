// #ifndef SC19_PICO_SC_UART_HPP_
// #define SC19_PICO_SC_UART_HPP_

// /**************************************************
//  * UART通信の出入力に関するコードです
//  * このファイルは，uart.cppに書かれている関数の一覧です
//  * 
//  * このファイルでは，UART通信に関するコードが宣言されています．
// **************************************************/

// //! @file uart.hpp
// //! @brief UARTでの入出力

// #include "sc_basic.hpp"
// #include "binary.hpp"

// #include "hardware/uart.h"


// namespace sc
// {

// //! @brief UART0かUART1か
// enum UART_ID
// {
//     uart_0,
//     uart_1
// };

// //! @brief  UARTのTXピン
// class TX : public Pin
// {
//     static constexpr uint8_t EnableUART0_TX[] = {0, 4, 8, 12, 16, 20};  // UART0のTXピンのGPIO番号が取り得る値
//     static constexpr uint8_t EnableUART1_TX[] = {2, 6, 10, 14, 18, 26};  // UART1のTXピンのGPIO番号が取り得る値
// public:
//     //! @brief TXピンを指定
//     //! @param tx_gpio TXピンのGPIO番号
//     TX(int tx_gpio);

//     //! @brief uartポートの種類を返す
//     //! @return uart0かuart1か
//     UART_ID get_uart_id() const;
// };

// //! @brief  UARTのRXピン
// class RX : public Pin
// {
//     static constexpr uint8_t EnableUART0_RX[] = {1, 5, 9, 13, 17, 21};  // UART0のRXピンのGPIO番号が取り得る値
//     static constexpr uint8_t EnableUART1_RX[] = {3, 7, 11, 15, 19, 27};  // UART1のRXピンのGPIO番号が取り得る値
// public:
//     //! @brief RXピンを指定
//     //! @param rx_gpio RXピンのGPIO番号
//     RX(int rx_gpio);

//     //! @brief uartポートの種類を返す
//     //! @return uart0かuart1か
//     UART_ID get_uart_id() const;
// };


// //! @brief UART通信
// class UART
// {
// private:
//     const TX _tx;  // UARTで使用するTXピン
//     const RX _rx;  // UARTで使用するRXピン
//     const Freq _freq;  // UARTの通信速度
//     const UART_ID _uart_id;  // UART0かUART1か

// public:
//     //! @brief UARTをセットアップ
//     //! @param tx UARTで使用するTXピン
//     //! @param rx UARTで使用するRXピン
//     //! @param freq UARTの通信速度 (10000_hz のように入力)
//     UART(TX tx, RX rx, Freq freq);

//     //! @brief UARTによる送信
//     //! @param output_data 送信するデータ
//     void write(Binary output_data) const;

//     //! @brief UARTによる受信
//     //! @return Binary型のバイト列
//     Binary read() const;

//     //! @brief UARTによるメモリへの送信
//     //! @param output_data 送信するデータ
//     //! @param slave_addr 通信先のデバイスのスレーブアドレス
//     //! @param memory_addr 通信先のデバイスのメモリの何番地にデータを記録するか
//     void write_memory(Binary output_data, SlaveAddr slave_addr, MemoryAddr memory_addr) const;

//     //! @brief UARTによるメモリからの受信
//     //! @param size 受信するバイト数
//     //! @param slave_addr 通信先のデバイスのスレーブアドレス
//     //! @param memory_addr 通信先のデバイスのメモリの何番地からデータを読み込むか
//     //! @return Binary型のバイト列
//     Binary read_memory(size_t size, SlaveAddr slave_addr, MemoryAddr memory_addr) const;
// private:
//     static bool IsUse[2];  // 既にUART0とUART1を使用しているか
// };
// bool UART::IsUse[2] = {false, false};

// }

// #endif  // SC19_PICO_SC_UART_HPP_