/**************************************************
 * UART通信の出入力に関するコードです
 * このファイルは，uart.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，UART通信に関するコードが定義されています．
**************************************************/

//! @file uart.cpp
//! @brief UARTでの入出力

#include "uart.hpp"

namespace sc
{

/***** class TX *****/

TX::TX(int tx_gpio):
    Pin(tx_gpio)
{
    if (gpio() != all_of(EnableUART0_TX) && gpio() != all_of(EnableUART1_TX))
    {
throw Error(__FILE__, __LINE__, "An incorrect TX pin number was entered");  // 正しくないTXピンの番号が入力されました
    }
}

UART_ID TX::get_uart_id() const
{
    if (gpio() == any_of(EnableUART0_TX))
    {
return UART_ID::uart_0;
    } else if (gpio() == any_of(EnableUART1_TX)) {
return UART_ID::uart_1;
    } else {
throw Error(__FILE__, __LINE__, "An incorrect TX pin number was entered");  // 正しくないTXピンの番号が入力されました
    }
}

/***** class RX *****/

RX::RX(int tx_gpio):
    Pin(tx_gpio)
{
    if (gpio() != all_of(EnableUART0_RX) && gpio() != all_of(EnableUART1_RX))
    {
throw Error(__FILE__, __LINE__, "An incorrect RX pin number was entered");  // 正しくないRXピンの番号が入力されました
    }
}

UART_ID RX::get_uart_id() const
{
    if (gpio() == any_of(EnableUART0_RX))
    {
return UART_ID::uart_0;
    } else if (gpio() == any_of(EnableUART1_RX)) {
return UART_ID::uart_1;
    } else {
throw Error(__FILE__, __LINE__, "An incorrect RX pin number was entered");  // 正しくないRXピンの番号が入力されました
    }
}


/***** class UART *****/



UART::UART(TX tx, RX rx, Frequency<Unit::Hz>  freq):
    _tx(tx), _rx(rx), _freq(freq), _uart_id(tx.get_uart_id())
{
    if (tx.get_uart_id() != rx.get_uart_id())
    {
throw Error(__FILE__, __LINE__, "An incorrect UART pin number was entered");  // 正しくないUARTのピン番号が入力されました
    } else if (!(Pin::Status.at(_tx.gpio()) == PinStatus::NoUse && Pin::Status.at(_rx.gpio()) == PinStatus::NoUse)) {
throw Error(__FILE__, __LINE__, "This pin is already in use");  // このピンは既に使用されています
    } else if (UART::IsUse[_uart_id]) {
throw Error(__FILE__, __LINE__, "UART cannot be reinitialized");  // UARTを再度初期化することはできません
    }

    Pin::Status.at(_tx.gpio()) = PinStatus::UartTx;
    Pin::Status.at(_rx.gpio()) = PinStatus::UartRx;

    UART::IsUse[_uart_id] = true;

    ::uart_init((_uart_id ? uart1 : uart0), static_cast<double>(_freq));  // pico-SDKの関数  UARTを初期化する

    ::gpio_set_function(_tx.gpio(), GPIO_FUNC_UART);  // pico-SDKの関数  ピンの機能をUARTモードにする
    ::gpio_set_function(_rx.gpio(), GPIO_FUNC_UART);  // pico-SDKの関数  ピンの機能をUARTモードにする

    // 割り込み処理の設定を行う
    if (_uart_id)
    {
        uart_set_hw_flow(uart1, false, false);  // フロー制御(受信準備が終わるまで送信しないで待つ機能)を無効にする
        uart_set_format(uart1, 8, 1, UART_PARITY_NONE);  // UART通信の設定をする
        uart_set_fifo_enabled(uart1, false);  // FIFO(受信したデータを一時的に保管する機能)をオフにし，1文字ずつ受信する
        irq_set_exclusive_handler(UART1_IRQ, uart1_handler);  // 割り込み処理で実行する関数をセット
        irq_set_enabled(UART1_IRQ, true);  // 割り込み処理を有効にする
    } else {
        uart_set_hw_flow(uart0, false, false);  // フロー制御(受信準備が終わるまで送信しないで待つ機能)を無効にする
        uart_set_format(uart0, 8, 1, UART_PARITY_NONE);  // UART通信の設定をする
        uart_set_fifo_enabled(uart0, false);  // FIFO(受信したデータを一時的に保管する機能)をオフにし，1文字ずつ受信する
        irq_set_exclusive_handler(UART0_IRQ, uart0_handler);  // 割り込み処理で実行する関数をセット
        irq_set_enabled(UART0_IRQ, true);  // 割り込み処理を有効にする
    }
}

void UART::write(Binary output_data) const
{
    ::uart_write_blocking((_uart_id ? uart1 : uart0), output_data, output_data.size());  // pico-SDKの関数  UARTで送信
}

Binary UART::read() const
{
    std::deque<uint8_t> other_data(0);
    std::swap(uart0_queue, other_data);
    return sc::Binary(other_data);  // 割り込み処理で一時保存しておいたデータを返す
}


//! @brief 割り込み処理でUART0の受信をする際に呼び出される関数
void UART::uart0_handler()
{
    while (uart_is_readable(uart0))
    {
        uart0_queue.push_back(uart_getc(uart0));  // 1文字読み込んで末尾に値を追加
    }
    if (uart0_queue.size() > MaxInputLen)
    {
        uart0_queue.erase(uart0_queue.begin(), uart0_queue.begin() + (uart0_queue.size() - MaxInputLen));  // データが多すぎるときは一部を削除
    }
}

//! @brief 割り込み処理でUART1の受信をする際に呼び出される関数
void UART::uart1_handler()
{
    while (uart_is_readable(uart1))
    {
        uart1_queue.push_back(uart_getc(uart1));  // 1文字読み込んで末尾に値を追加
    }
    if (uart1_queue.size() > MaxInputLen)
    {
        uart1_queue.erase(uart1_queue.begin(), uart1_queue.begin() + (uart1_queue.size() - MaxInputLen));  // データが多すぎるときは一部を削除
    }
}


}
