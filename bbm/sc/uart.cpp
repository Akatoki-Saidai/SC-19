// /**************************************************
//  * UART通信の出入力に関するコードです
//  * このファイルは，uart.hppに名前だけ書かれている関数の中身です
//  * 
//  * このファイルでは，UART通信に関するコードが定義されています．
// **************************************************/

// //! @file uart.cpp
// //! @brief UARTでの入出力

// #include "uart.hpp"

// #include <vector>

// namespace sc
// {

// /***** class TX *****/

// TX::TX(int tx_gpio):
//     Pin(tx_gpio)
// {
//     if (get_gpio() != all_of(EnableUART0_TX) && get_gpio() != all_of(EnableUART1_TX))
//     {
// throw Error(__FILE__, __LINE__, "An incorrect TX pin number was entered");  // 正しくないTXピンの番号が入力されました
//     }
// }

// UART_ID TX::get_uart_id() const
// {
//     if (get_gpio() == any_of(EnableUART0_TX))
//     {
// return UART_ID::uart_0;
//     } else if (get_gpio() == any_of(EnableUART1_TX)) {
// return UART_ID::uart_1;
//     } else {
// throw Error(__FILE__, __LINE__, "An incorrect TX pin number was entered");  // 正しくないTXピンの番号が入力されました
//     }
// }

// /***** class RX *****/

// RX::RX(int tx_gpio):
//     Pin(tx_gpio)
// {
//     if (get_gpio() != all_of(EnableUART0_RX) && get_gpio() != all_of(EnableUART1_RX))
//     {
// throw Error(__FILE__, __LINE__, "An incorrect RX pin number was entered");  // 正しくないRXピンの番号が入力されました
//     }
// }

// UART_ID RX::get_uart_id() const
// {
//     if (get_gpio() == any_of(EnableUART0_RX))
//     {
// return UART_ID::uart_0;
//     } else if (get_gpio() == any_of(EnableUART1_RX)) {
// return UART_ID::uart_1;
//     } else {
// throw Error(__FILE__, __LINE__, "An incorrect RX pin number was entered");  // 正しくないRXピンの番号が入力されました
//     }
// }


// /***** class UART *****/

// UART::UART(TX tx, RX rx):
//     UART(tx, rx, 10'000_hz) {}

// UART::UART(TX tx, RX rx, Freq freq):
//     _tx(tx), _rx(rx), _freq(freq), _uart_id(tx.get_uart_id())
// {
//     if (tx.get_uart_id() != rx.get_uart_id())
//     {
// throw Error(__FILE__, __LINE__, "An incorrect UART pin number was entered");  // 正しくないUARTのピン番号が入力されました
//     } else if (!(Pin::Status.at(_tx.get_gpio()) == PinStatus::NoUse && Pin::Status.at(_rx.get_gpio()) == PinStatus::NoUse))
//     {
// throw Error(__FILE__, __LINE__, "This pin is already in use");  // このピンは既に使用されています
//     } else if (UART::IsUse[_uart_id])
//     {
// throw Error(__FILE__, __LINE__, "UART cannot be reinitialized");  // UARTを再度初期化することはできません
//     }

//     Pin::Status.at(_tx.get_gpio()) = PinStatus::UartTx;
//     Pin::Status.at(_rx.get_gpio()) = PinStatus::UartRx;

//     UART::IsUse[_uart_id] = true;

//     ::uart_init((_uart_id ? uart1 : uart0), static_cast<double>(_freq));  // pico-SDKの関数  UARTを初期化する

//     ::gpio_set_function(_tx.get_gpio(), GPIO_FUNC_UART);  // pico-SDKの関数  ピンの機能をUARTモードにする
//     ::gpio_pull_up(_tx.get_gpio());  // pico-SDKの関数  プルアップ抵抗を有効にする
//     ::gpio_set_function(_rx.get_gpio(), GPIO_FUNC_UART);  // pico-SDKの関数  ピンの機能をUARTモードにする
//     ::gpio_pull_up(_rx.get_gpio());  // pico-SDKの関数  プルアップ抵抗を有効にする
// }

// void UART::write(Binary output_data, SlaveAddr slave_addr) const
// {
//     ::uart_write_blocking((_uart_id ? uart1 : uart0), slave_addr, output_data, output_data.size(), false);  // pico-SDKの関数  UARTで送信
// }

// Binary UART::read(size_t size, SlaveAddr slave_addr) const
// {
//     std::vector<uint8_t> input_data(size);
//     size_t input_size = 0;  // 実際には何バイト受信したか
//     input_size = ::uart_read_blocking((_uart_id ? uart1 : uart0), slave_addr, input_data.data(), size, false);  // pico-SDKの関数  UARTで受信
//     input_data.resize(input_size);
//     return Binary(input_data);
// }

// void UART::write_memory(Binary output_data, SlaveAddr slave_addr, MemoryAddr memory_addr) const
// {
//     Binary corrected_data = memory_addr + output_data;
//     ::uart_write_blocking((_uart_id ? uart1 : uart0), slave_addr, corrected_data, corrected_data.size(), false);  // pico-SDKの関数  UARTで送信
// }

// Binary UART::read_memory(size_t size, SlaveAddr slave_addr, MemoryAddr memory_addr) const
// {
//     std::vector<uint8_t> input_data(size);
//     size_t input_size = 0;  // 実際には何バイト受信したか
//     uint8_t output_data = memory_addr;
//     ::uart_write_blocking((_uart_id ? uart1 : uart0), slave_addr, &output_data, 1, true);  // まず，メモリアドレスを送信
//     input_size = ::uart_read_blocking((_uart_id ? uart1 : uart0), slave_addr, input_data.data(), size, false);  // pico-SDKの関数  UARTで受信
//     input_data.resize(input_size);
//     return Binary(input_data);
// }

// }
