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
}

void Flush::clear()
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

    _target_offset = _target_begin;
    _write_data.fill(0);
}

void Flush::write(const Binary& write_binary)
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    static std::size_t _write_index = 0;

    const uint8_t* ptr = write_binary;
    std::size_t write_size = write_binary.size();
    for (std::size_t i=0; i<write_size; ++i)
    {
        if (_write_index < _write_data.size())
        {
            _write_data.at(_write_index) = *(ptr + i);
        } else {
            // 割り込み無効にする
            uint32_t ints = save_and_disable_interrupts();
            // Flash書き込み。
            //  書込単位はflash.hで定義されている FLASH_PAGE_SIZE(256Byte) の倍数とする
            flash_range_program(_target_offset, _write_data.data(), _write_data.size());
            // 割り込みフラグを戻す
            restore_interrupts(ints);
            _target_offset += _write_data.size();
            if (_target_offset > _target_end)
            {
                _target_offset = _target_begin;
            }

            _write_data.fill(0U);
            _write_data.at(0) = *(ptr + 0);
            _write_index = 0;
        }
        ++_write_index;
    }
}

Flush::~Flush()
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    this->write(std::string("\nlog end ") + __DATE__ + __TIME__);
    flash_range_program(_target_offset, _write_data.data(), _write_data.size());
}

void Flush::print()
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    std::cout << "\n#################### Log Data ####################" << std::endl;
    for (uint32_t i=_target_begin; i<_target_end; ++i)
    {
        std::cout << *(const uint8_t *) (XIP_BASE + i);
    }
    std::cout << std::endl;
    std::cout << "##################################################\n" << std::endl;
}

}