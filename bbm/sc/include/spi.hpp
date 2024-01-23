#ifndef SC19_PICO_SC_SPI_HPP_
#define SC19_PICO_SC_SPI_HPP_

/**************************************************
 * SPI通信の出入力に関するコードです
 * このファイルは，spi.cppに書かれている関数の一覧です
 * 
 * このファイルでは，SPIのマスター側の通信に関するコードが宣言されています．
**************************************************/

//! @file spi.hpp
//! @brief SPIでの入出力

#include "gpio.hpp"

#include "sc_basic.hpp"
#include "binary.hpp"

#include "hardware/spi.h"


namespace sc
{

    
//! @brief SPI0かSPI1か
enum SPI_ID
{
    spi_0,
    spi_1
};

//! @brief  SPIのMISOピン
class MISO : public Pin
{
    static constexpr uint8_t EnableSPI0_MISO[] = {0, 4, 16};  // SPI0のMISOピンのGPIO番号が取り得る値
    static constexpr uint8_t EnableSPI1_MISO[] = {8, 12};  // SPI1のMISOピンのGPIO番号が取り得る値
public:
    //! @brief MISOピンを指定
    //! @param miso_gpio MISOピンのGPIO番号
    MISO(int miso_gpio);

    //! @brief spiポートの種類を返す
    //! @return spi0かspi1か
    SPI_ID get_spi_id() const;
};
using RX = MISO;

//! @brief CSピンを指定
class CS
{
    const std::vector<Pin> _cs_pins;
public:
    //! @brief CSピンを指定
    //! @param cs_gpios CSピンのGPIO番号を(3, 4, 5)のように並べて入力
    template<typename... T>
    CS(T... cs_gpios):
        _cs_pins(Pin(cs_gpios)...) {}

    //! @brief CSピンを指定
    //! @param cs_gpios CSピンのGPIO番号を{3, 4, 5}のように並べて入力
    CS(std::initializer_list<Pin> cs_gpios):
        _cs_pins(cs_gpios) {}

    //! @brief CSピンを取得
    operator std::vector<Pin>() const
        {return _cs_pins;}
    const std::vector<Pin>& get() const 
        {return _cs_pins;}

    //! @brief 全てのピンが未使用であるかを確かめる
    bool no_use() const;

    //! @brief 保存されているCSピンの数を取得
    size_t size() const
        {return _cs_pins.size();}
};

//! @brief  SPIのMSCKピン
class SCK : public Pin
{
    static constexpr uint8_t EnableSPI0_SCK[] = {2, 6, 18};  // SPI0のSCKピンのGPIO番号が取り得る値
    static constexpr uint8_t EnableSPI1_SCK[] = {10, 14};  // SPI1のSCKピンのGPIO番号が取り得る値
public:
    //! @brief SCKピンを指定
    //! @param sck_gpio SCKピンのGPIO番号
    SCK(int sck_gpio);

    //! @brief spiポートの種類を返す
    //! @return spi0かspi1か
    SPI_ID get_spi_id() const;
};

//! @brief  SPIのMOSIピン
class MOSI : public Pin
{
    static constexpr uint8_t EnableSPI0_MOSI[] = {3, 7, 19};  // SPI0のMOSIピンのGPIO番号が取り得る値
    static constexpr uint8_t EnableSPI1_MOSI[] = {11, 15};  // SPI1のMOSIピンのGPIO番号が取り得る値
public:
    //! @brief MOSIピンを指定
    //! @param mosi_gpio MOSIピンのGPIO番号
    MOSI(int mosi_gpio);

    //! @brief spiポートの種類を返す
    //! @return spi0かspi1か
    SPI_ID get_spi_id() const;
};
using TX = MOSI;


//! @brief SPI通信
class SPI
{
public:

    //! @brief 通信先のデバイス内のメモリのアドレス
    class MemoryAddr
    {
        const uint8_t _memory_addr;
        static constexpr uint8_t MaxMemoryAddr = 0b01111111;
    public:
        MemoryAddr(unsigned int memory_addr);

        operator uint8_t() const;

        //! @brief メモリアドレスへのポインタ
        //! @note ポインタを介して値を変更した場合，動作は未定義
        const uint8_t* operator&() const
            {return &_memory_addr;}
    };

private:
    const MISO _miso;  // SPIで使用するMISOピン
    const SCK _sck;  // SPIで使用するSCKピン
    const MOSI _mosi;  // SPIで使用するMOSIピン
    const Freq _freq;  // SPIの通信速度
    const SPI_ID _spi_id;  // SPI0かSPI1か
    std::vector<GPIO<Out>> _cs_pins;  // SPIで使用する全てのCSピン

public:
    //! @brief SPIをセットアップ
    //! @param miso SPIで使用するMISOピン
    //! @param cs SPIで使用するすべてのCSピン
    //! @param sck SPIで使用するSCKピン
    //! @param mosi SPIで使用するMOSIピン
    SPI(MISO miso, CS cs, SCK sck, MOSI mosi);

    //! @brief SPIをセットアップ
    //! @param miso SPIで使用するMISOピン
    //! @param cs SPIで使用するすべてのCSピン
    //! @param sck SPIで使用するSCKピン
    //! @param mosi SPIで使用するMOSIピン
    //! @param freq SPIの通信速度 (10000_hz のように入力)
    SPI(MISO miso, CS cs, SCK sck, MOSI mosi, Freq freq);

    //! @brief SPIによる送信
    //! @param output_data 送信するデータ
    //! @param cs_pin 通信先のデバイスのチップセレクトピンの番号
    void write(Binary output_data, CS cs_pin) const;

    //! @brief SPIによる受信
    //! @param size 受信するバイト数
    //! @param cs_pin 通信先のデバイスのチップセレクトピンの番号
    //! @return Binary型のバイト列
    Binary read(size_t size, CS cs_pin) const;

    //! @brief SPIによるメモリへの送信
    //! @param output_data 送信するデータ
    //! @param cs_pin 通信先のデバイスのチップセレクトピンの番号
    //! @param memory_addr 通信先のデバイスのメモリの何番地にデータを記録するか
    void write_memory(Binary output_data, CS cs_pin, MemoryAddr memory_addr) const;

    //! @brief SPIによるメモリからの受信
    //! @param size 受信するバイト数
    //! @param cs_pin 通信先のデバイスのチップセレクトピンの番号
    //! @param memory_addr 通信先のデバイスのメモリの何番地からデータを読み込むか
    //! @return Binary型のバイト列
    Binary read_memory(size_t size, CS cs_pin, MemoryAddr memory_addr) const;
private:
    static bool IsUse[2];  // 既にSPI0とSPI1を使用しているか
};

}

#endif  // SC19_PICO_SC_SPI_HPP_