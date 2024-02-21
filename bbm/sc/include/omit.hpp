#ifndef SC19_PICO_SC_OMIT_HPP_
#define SC19_PICO_SC_OMIT_HPP_

/**************************************************
 * このプログラム群で使われているクラスなどの別名を宣言
 * 
 * I2C::SDAを単にSDAと入力できるようにするといった，別名や省略に関するコードです
**************************************************/

//! @file omit.hpp
//! @brief 別名や省略に関するコード

#include "sc_basic.hpp"

#include "gpio.hpp"
#include "i2c.hpp"
#include "spi.hpp"


namespace sc
{

//! @brief 通信先のデバイス内のメモリのアドレス
//! @note I2CやSPIで通信する際に，通信先デバイスのメモリの何番地からデータを読み込むのか，書き込むのかを指定
class MemoryAddr
{
    unsigned int _memory_addr;
public:
    explicit MemoryAddr(unsigned int memory_addr):
        _memory_addr(memory_addr) 
    {
        #ifdef DEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
    }
    
    operator I2C::MemoryAddr() const
        {
            #ifdef DEBUG
                std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
            #endif
            return I2C::MemoryAddr(_memory_addr);
        }

    operator SPI::MemoryAddr() const
    {
        #ifdef DEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        return SPI::MemoryAddr(_memory_addr);
    }
};

}

#endif // SC19_PICO_SC_OMIT_HPP_