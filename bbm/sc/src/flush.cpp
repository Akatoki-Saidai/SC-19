/**************************************************
 * マイコンのフラッシュメモリに直接データを保存するためのコードです
 * このファイルは，flush.hppに名前だけ書かれている関数の中身です
 * 
**************************************************/

//! @file flush.cpp
//! @brief マイコンのフラッシュメモリに直接データを保存

#include "flush.hpp"

namespace sc
{

Flush::Flush()
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    // 割り込み無効にする
    uint32_t ints = save_and_disable_interrupts();
    // Flash消去。
    //  消去単位はflash.hで定義されている FLASH_SECTOR_SIZE(4096Byte) の倍数とする
    flash_range_erase(_target_begin, _block_size * _block_num);
    // 割り込みフラグを戻す
    restore_interrupts(ints);
}

void Flush::write(const Binary& write_binary)
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    static std::size_t write_index = 0;

    const uint8_t* ptr = write_binary;
    for (std::size_t i=0; i<write_binary.size(); ++i)
    {
        if (write_index < write_data.size())
        {
            write_data.at(write_index) = *(ptr + i);
        } else {
            // 割り込み無効にする
            uint32_t ints = save_and_disable_interrupts();
            // Flash書き込み。
            //  書込単位はflash.hで定義されている FLASH_PAGE_SIZE(256Byte) の倍数とする
            flash_range_program(_target_offset, write_data.data(), write_data.size());
            // 割り込みフラグを戻す
            restore_interrupts(ints);
            _target_offset += write_data.size();
        }
        ++write_index;
    }
}

const uint8_t* Flush::read()
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return (const uint8_t *) (XIP_BASE + _target_begin);
}

void Flush::print()
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    for (std::size_t i=_target_begin; i<=_target_end; ++i)
    {
        std::cout << *(const uint8_t *) (XIP_BASE + i);
    }
    std::cout << std::endl;
}

}