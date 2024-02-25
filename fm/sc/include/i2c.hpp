#ifndef SC19_PICO_SC_I2C_HPP_
#define SC19_PICO_SC_I2C_HPP_

/**************************************************
 * I2C通信の出入力に関するコードです
 * このファイルは，i2c.cppに書かれている関数の一覧です
 * 
 * このファイルでは，I2Cのマスター側の通信に関するコードが宣言されています．
**************************************************/

//! @file i2c.hpp
//! @brief I2Cでの入出力

#include "sc_basic.hpp"
#include "binary.hpp"

#include "hardware/i2c.h"


namespace sc
{

//! @brief I2C0かI2C1か
enum I2C_ID
{
    i2c_0,
    i2c_1
};

//! @brief  I2CのSDAピン
class SDA : public Pin
{
    static constexpr uint8_t EnableI2C0_SDA[] = {0, 4, 8, 12, 16, 20};  // I2C0のSDAピンのGPIO番号が取り得る値
    static constexpr uint8_t EnableI2C1_SDA[] = {2, 6, 10, 14, 18, 26};  // I2C1のSDAピンのGPIO番号が取り得る値
public:
    //! @brief SDAピンを指定
    //! @param sda_gpio SDAピンのGPIO番号
    SDA(int sda_gpio);

    //! @brief i2cポートの種類を返す
    //! @return i2c0かi2c1か
    I2C_ID get_i2c_id() const;
};

//! @brief  I2CのSCLピン
class SCL : public Pin
{
    static constexpr uint8_t EnableI2C0_SCL[] = {1, 5, 9, 13, 17, 21};  // I2C0のSCLピンのGPIO番号が取り得る値
    static constexpr uint8_t EnableI2C1_SCL[] = {3, 7, 11, 15, 19, 27};  // I2C1のSCLピンのGPIO番号が取り得る値
public:
    //! @brief SCLピンを指定
    //! @param scl_gpio SCLピンのGPIO番号
    SCL(int scl_gpio);

    //! @brief i2cポートの種類を返す
    //! @return i2c0かi2c1か
    I2C_ID get_i2c_id() const;
};

//! @brief I2Cのスレーブアドレス (通信先のデバイスのID)
class SlaveAddr
{
    const uint8_t _slave_addr;
public:
    //! @brief I2Cのスレーブアドレス (通信先のデバイスのID)
    SlaveAddr(uint8_t slave_addr);

    //! @brief スレーブアドレスを取得
    uint8_t get_addr() const;

    //! @brief スレーブアドレス
    operator uint8_t() const;
};


//! @brief I2C通信
class I2C : Noncopyable
{
public:
    //! @brief 通信先のデバイス内のメモリのアドレス
    class MemoryAddr
    {
        const uint8_t _memory_addr;
        static constexpr uint8_t MaxMemoryAddr = 0b11111111;
    public:
        MemoryAddr(unsigned int memory_addr);

        operator uint8_t() const;
    };

private:
    const SDA _sda;  // I2Cで使用するSDAピン
    const SCL _scl;  // I2Cで使用するSCLピン
    const Frequency<Unit::Hz> _freq;  // I2Cの通信速度
    const I2C_ID _i2c_id;  // I2C0かI2C1か

public:
    //! @brief I2Cをセットアップ
    //! @param sda I2Cで使用するSDAピン
    //! @param scl I2Cで使用するSCLピン
    I2C(SDA sda, SCL scl);

    //! @brief I2Cをセットアップ
    //! @param sda I2Cで使用するSDAピン
    //! @param scl I2Cで使用するSCLピン
    //! @param freq I2Cの通信速度 (10000_hz のように入力)
    I2C(SDA sda, SCL scl, Frequency<Unit::Hz> freq);

    //! @brief I2Cによる送信
    //! @param output_data 送信するデータ
    //! @param slave_addr 通信先のデバイスのスレーブアドレス (誰に送信するか)
    void write(Binary output_data, SlaveAddr slave_addr) const;

    //! @brief I2Cによる受信
    //! @param size 受信するバイト数
    //! @param slave_addr 通信先のデバイスのスレーブアドレス (誰から受信するか)
    //! @return Binary型のバイト列
    Binary read(std::size_t size, SlaveAddr slave_addr) const;

    //! @brief I2Cによるメモリへの送信
    //! @param output_data 送信するデータ
    //! @param slave_addr 通信先のデバイスのスレーブアドレス
    //! @param memory_addr 通信先のデバイスのメモリの何番地にデータを記録するか
    void write_memory(Binary output_data, SlaveAddr slave_addr, MemoryAddr memory_addr) const;

    //! @brief I2Cによるメモリからの受信
    //! @param size 受信するバイト数
    //! @param slave_addr 通信先のデバイスのスレーブアドレス
    //! @param memory_addr 通信先のデバイスのメモリの何番地からデータを読み込むか
    //! @return Binary型のバイト列
    Binary read_memory(std::size_t size, SlaveAddr slave_addr, MemoryAddr memory_addr) const;
private:
    static inline bool IsUse[2] = {false, false};  // 既にI2C0とI2C1を使用しているか
};

}

#endif  // SC19_PICO_SC_I2C_HPP_