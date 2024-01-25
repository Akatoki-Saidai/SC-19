/**************************************************
 * I2C通信の出入力に関するコードです
 * このファイルは，i2c.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，I2Cのマスター側の通信に関するコードが定義されています．
**************************************************/

//! @file i2c.cpp
//! @brief I2Cでの入出力

#include "i2c.hpp"

#include <vector>

namespace sc
{

/***** class SDA *****/

SDA::SDA(int sda_gpio):
    Pin(sda_gpio)
{
    if (get_gpio() != all_of(EnableI2C0_SDA) && get_gpio() != all_of(EnableI2C1_SDA))
    {
throw Error(__FILE__, __LINE__, "An incorrect SDA pin number was entered");  // 正しくないSDAピンの番号が入力されました
    }
}

I2C_ID SDA::get_i2c_id() const
{
    if (get_gpio() == any_of(EnableI2C0_SDA))
    {
return I2C_ID::i2c_0;
    } else if (get_gpio() == any_of(EnableI2C1_SDA)) {
return I2C_ID::i2c_1;
    } else {
throw Error(__FILE__, __LINE__, "An incorrect SDA pin number was entered");  // 正しくないSDAピンの番号が入力されました
    }
}

/***** class SCL *****/

SCL::SCL(int sda_gpio):
    Pin(sda_gpio)
{
    if (get_gpio() != all_of(EnableI2C0_SCL) && get_gpio() != all_of(EnableI2C1_SCL))
    {
throw Error(__FILE__, __LINE__, "An incorrect SCL pin number was entered");  // 正しくないSCLピンの番号が入力されました
    }
}

I2C_ID SCL::get_i2c_id() const
{
    if (get_gpio() == any_of(EnableI2C0_SCL))
    {
return I2C_ID::i2c_0;
    } else if (get_gpio() == any_of(EnableI2C1_SCL)) {
return I2C_ID::i2c_1;
    } else {
throw Error(__FILE__, __LINE__, "An incorrect SCL pin number was entered");  // 正しくないSCLピンの番号が入力されました
    }
}

/***** class SlaveAddr *****/

SlaveAddr::SlaveAddr(uint8_t slave_addr):
    _slave_addr(slave_addr)
{
    if (0b10000000 <= _slave_addr)
    {
throw Error(__FILE__, __LINE__, "An incorrect I2C slave address was entered");  // 正しくないI2Cのスレーブアドレスが入力されました
    }
}

uint8_t SlaveAddr::get_addr() const
{
    return _slave_addr;
}

SlaveAddr::operator uint8_t() const
{
    return _slave_addr;
}

/***** class I2C::MemoryAddr *****/

I2C::MemoryAddr::MemoryAddr(unsigned int memory_addr):
    _memory_addr(static_cast<uint8_t>(memory_addr))
{
    if (MaxMemoryAddr < memory_addr)
    {
throw Error(__FILE__, __LINE__, "An incorrect memory address was entered");  // 正しくないメモリアドレスが入力されました 
    }
}

I2C::MemoryAddr::operator uint8_t() const
{
    return _memory_addr;
}

/***** class I2C *****/

I2C::I2C(SDA sda, SCL scl):
    I2C(sda, scl, 10'000_hz) {}

I2C::I2C(SDA sda, SCL scl, Freq freq):
    _sda(sda), _scl(scl), _freq(freq), _i2c_id(sda.get_i2c_id())
{
    if (sda.get_i2c_id() != scl.get_i2c_id())
    {
throw Error(__FILE__, __LINE__, "An incorrect I2C pin number was entered");  // 正しくないI2Cのピン番号が入力されました
    } else if (!(Pin::Status.at(_sda.get_gpio()) == PinStatus::NoUse && Pin::Status.at(_scl.get_gpio()) == PinStatus::NoUse))
    {
throw Error(__FILE__, __LINE__, "This pin is already in use");  // このピンは既に使用されています
    } else if (I2C::IsUse[_i2c_id])
    {
throw Error(__FILE__, __LINE__, "I2C cannot be reinitialized");  // I2Cを再度初期化することはできません
    }

    Pin::Status.at(_sda.get_gpio()) = PinStatus::I2cSda;
    Pin::Status.at(_scl.get_gpio()) = PinStatus::I2cScl;

    I2C::IsUse[_i2c_id] = true;

    ::i2c_init((_i2c_id ? i2c1 : i2c0), static_cast<double>(_freq));  // pico-SDKの関数  I2Cを初期化する

    ::gpio_set_function(_sda.get_gpio(), GPIO_FUNC_I2C);  // pico-SDKの関数  ピンの機能をI2Cモードにする
    ::gpio_pull_up(_sda.get_gpio());  // pico-SDKの関数  プルアップ抵抗を有効にする
    ::gpio_set_function(_scl.get_gpio(), GPIO_FUNC_I2C);  // pico-SDKの関数  ピンの機能をI2Cモードにする
    ::gpio_pull_up(_scl.get_gpio());  // pico-SDKの関数  プルアップ抵抗を有効にする
}

void I2C::write(Binary output_data, SlaveAddr slave_addr) const
{
    ::i2c_write_blocking((_i2c_id ? i2c1 : i2c0), slave_addr, output_data, output_data.size(), false);  // pico-SDKの関数  I2Cで送信
}

Binary I2C::read(size_t size, SlaveAddr slave_addr) const
{
    std::vector<uint8_t> input_data(size);
    size_t input_size = 0;  // 実際には何バイト受信したか
    input_size = ::i2c_read_blocking((_i2c_id ? i2c1 : i2c0), slave_addr, input_data.data(), size, false);  // pico-SDKの関数  I2Cで受信
    input_data.resize(input_size);
    return Binary(input_data);
}

void I2C::write_memory(Binary output_data, SlaveAddr slave_addr, MemoryAddr memory_addr) const
{
    Binary corrected_data = memory_addr + output_data;
    ::i2c_write_blocking((_i2c_id ? i2c1 : i2c0), slave_addr, corrected_data, corrected_data.size(), false);  // pico-SDKの関数  I2Cで送信
}

Binary I2C::read_memory(size_t size, SlaveAddr slave_addr, MemoryAddr memory_addr) const
{
    std::vector<uint8_t> input_data(size);
    size_t input_size = 0;  // 実際には何バイト受信したか
    uint8_t output_data = memory_addr;
    ::i2c_write_blocking((_i2c_id ? i2c1 : i2c0), slave_addr, &output_data, 1, true);  // まず，メモリアドレスを送信
    input_size = ::i2c_read_blocking((_i2c_id ? i2c1 : i2c0), slave_addr, input_data.data(), size, false);  // pico-SDKの関数  I2Cで受信
    input_data.resize(input_size);
    return Binary(input_data);
}

bool I2C::IsUse[2] = {false, false};

}
