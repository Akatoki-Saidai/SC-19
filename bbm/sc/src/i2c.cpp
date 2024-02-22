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

SDA::SDA(int sda_gpio) try :
    Pin(sda_gpio)
{
    if (gpio() != all_of(EnableI2C0_SDA) && gpio() != all_of(EnableI2C1_SDA))
    {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect SDA pin number was entered"));  // 正しくないSDAピンの番号が入力されました
    }
}
catch (const std::exception& e)
{
    print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
    print(e.what());
}

I2C_ID SDA::get_i2c_id() const
{
    if (gpio() == any_of(EnableI2C0_SDA))
    {
return I2C_ID::i2c_0;
    } else if (gpio() == any_of(EnableI2C1_SDA)) {
return I2C_ID::i2c_1;
    } else {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect SDA pin number was entered"));  // 正しくないSDAピンの番号が入力されました
    }
}

/***** class SCL *****/

SCL::SCL(int sda_gpio) try :
    Pin(sda_gpio)
{
    if (gpio() != all_of(EnableI2C0_SCL) && gpio() != all_of(EnableI2C1_SCL))
    {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect SCL pin number was entered"));  // 正しくないSCLピンの番号が入力されました
    }
}
catch (const std::exception& e)
{
    print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
    print(e.what());
}

I2C_ID SCL::get_i2c_id() const
{
    if (gpio() == any_of(EnableI2C0_SCL))
    {
return I2C_ID::i2c_0;
    } else if (gpio() == any_of(EnableI2C1_SCL)) {
return I2C_ID::i2c_1;
    } else {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect SCL pin number was entered"));  // 正しくないSCLピンの番号が入力されました
    }
}

/***** class SlaveAddr *****/

SlaveAddr::SlaveAddr(uint8_t slave_addr):
    _slave_addr(slave_addr)
{
    if (0b10000000 <= _slave_addr)
    {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect I2C slave address was entered"));  // 正しくないI2Cのスレーブアドレスが入力されました
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
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect memory address was entered"));  // 正しくないメモリアドレスが入力されました 
    }
}

I2C::MemoryAddr::operator uint8_t() const
{
    return _memory_addr;
}

/***** class I2C *****/

I2C::I2C(SDA sda, SCL scl):
    I2C(sda, scl, 10000_hz)
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
}

I2C::I2C(SDA sda, SCL scl, Frequency<Unit::Hz> freq) try :
    _sda(sda), _scl(scl), _freq(freq), _i2c_id(sda.get_i2c_id())
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (sda.get_i2c_id() != scl.get_i2c_id())
    {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect I2C pin number was entered"));  // 正しくないI2Cのピン番号が入力されました
    } else if (!(Pin::Status.at(_sda.gpio()) == PinStatus::NoUse && Pin::Status.at(_scl.gpio()) == PinStatus::NoUse)) {
throw std::logic_error(f_err(__FILE__, __LINE__, "This pin is already in use"));  // このピンは既に使用されています
    } else if (I2C::IsUse[_i2c_id]) {
throw std::logic_error(f_err(__FILE__, __LINE__, "I2C cannot be reinitialized"));  // I2Cを再度初期化することはできません
    }

    Pin::Status.at(_sda.gpio()) = PinStatus::I2cSda;
    Pin::Status.at(_scl.gpio()) = PinStatus::I2cScl;

    I2C::IsUse[_i2c_id] = true;

    ::i2c_init((_i2c_id ? i2c1 : i2c0), static_cast<double>(_freq));  // pico-SDKの関数  I2Cを初期化する

    ::gpio_set_function(_sda.gpio(), GPIO_FUNC_I2C);  // pico-SDKの関数  ピンの機能をI2Cモードにする
    ::gpio_pull_up(_sda.gpio());  // pico-SDKの関数  プルアップ抵抗を有効にする
    ::gpio_set_function(_scl.gpio(), GPIO_FUNC_I2C);  // pico-SDKの関数  ピンの機能をI2Cモードにする
    ::gpio_pull_up(_scl.gpio());  // pico-SDKの関数  プルアップ抵抗を有効にする
}
catch(const std::exception& e)
{
    print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
    print(e.what());
}

void I2C::write(Binary output_data, SlaveAddr slave_addr) const
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    int output_size = 0;  // 実際には何バイト送信したか
    ::i2c_write_blocking_until((_i2c_id ? i2c1 : i2c0), slave_addr, output_data, output_data.size(), false, make_timeout_time_us(100*1000));  // pico-SDKの関数  I2Cで送信
    if (output_size < 0)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Failed to transmit via I2C. SlaveAddr:%hhx", slave_addr));  // I2Cによる送信に失敗しました
    }
}

Binary I2C::read(std::size_t size, SlaveAddr slave_addr) const
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    std::vector<uint8_t> input_data(size);

    int input_size = 0;  // 実際には何バイト受信したか
    input_size = ::i2c_read_blocking_until((_i2c_id ? i2c1 : i2c0), slave_addr, input_data.data(), size, false, make_timeout_time_us(100*1000));  // pico-SDKの関数  I2Cで受信
    if (input_size < 0)
    {
        // input_size = 0;
throw std::runtime_error(f_err(__FILE__, __LINE__, "Failed to receive via I2C. SlaveAddr:%hhx", slave_addr));  // I2Cによる受信に失敗しました
    }
    input_data.resize(input_size);
    return Binary(input_data);
}

void I2C::write_memory(Binary output_data, SlaveAddr slave_addr, MemoryAddr memory_addr) const
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    Binary corrected_data = memory_addr + output_data;
    int output_size = 0;  // 実際には何バイト送信したか
    output_size = ::i2c_write_blocking_until((_i2c_id ? i2c1 : i2c0), slave_addr, corrected_data, corrected_data.size(), false, make_timeout_time_us(100*1000));  // pico-SDKの関数  I2Cで送信
    if (output_size < 0)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Failed to transmit via I2C. SlaveAddr:%hhx, MemoryAddr:%hhx", slave_addr, memory_addr));  // I2Cによる送信に失敗しました
    }
}

Binary I2C::read_memory(std::size_t size, SlaveAddr slave_addr, MemoryAddr memory_addr) const
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    std::vector<uint8_t> input_data(size);
    int output_size = 0;  // 実際には何バイト送信したか
    int input_size = 0;  // 実際には何バイト受信したか
    uint8_t output_data = memory_addr;
    output_size = ::i2c_write_blocking_until((_i2c_id ? i2c1 : i2c0), slave_addr, &output_data, 1, true, make_timeout_time_us(100*1000));  // まず，メモリアドレスを送信
    input_size = ::i2c_read_blocking_until((_i2c_id ? i2c1 : i2c0), slave_addr, input_data.data(), size, false, make_timeout_time_us(100*1000));  // pico-SDKの関数  I2Cで受信
    if (input_size < 0 || output_size < 0)
    {
        // input_size = 0;
throw std::runtime_error(f_err(__FILE__, __LINE__, "Failed to receive via I2C. SlaveAddr:%hhx, MemoryAddr:%hhx", slave_addr, memory_addr));  // I2Cによる受信に失敗しました
    }
    input_data.resize(input_size);
    return Binary(input_data);
}

}
