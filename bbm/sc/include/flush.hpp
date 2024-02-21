#ifndef SC19_PICO_SC_FLUSH_HPP_
#define SC19_PICO_SC_FLUSH_HPP_ 

/**************************************************
 * マイコンのフラッシュメモリに直接データを保存するためのコードです
 * このファイルは，flush.cppに書かれている関数の一覧です
 * 
**************************************************/

//! @file flush.hpp
//! @brief マイコンのフラッシュメモリに直接データを保存

#include "sc_basic.hpp"

#include "hardware/flash.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"

#include "binary.hpp"

namespace sc
{

class Flush : Noncopyable
{
    static constexpr uint32_t _target_begin = 0x1F0000;  // W25Q16JVの最終ブロック(Block31)のセクタ0の先頭アドレス = 0x1F0000
    static constexpr uint32_t _target_end = 0x1FFFFF;
    static constexpr uint32_t _block_size = 0xFFFF;  // 1ブロックのサイズ
    static constexpr uint8_t _block_num = 1;  // 使用するブロックの数
    uint32_t _target_offset = _target_begin;
    std::array<uint8_t, FLASH_PAGE_SIZE> _write_data;
public:
    //! @brief フラッシュメモリのセットアップ
    Flush();

    void write(const Binary& write_data);

    void print();

    void clear();

    ~Flush();
};

// uint8_t g_read_data[3];

// void load_setting_from_flash(void)
// {
//     // W25Q16JVの最終ブロック(Block31)のセクタ0の先頭アドレス = 0x1F0000
//     const uint32_t FLASH_TARGET_OFFSET = 0x1F0000;
//     // XIP_BASE(0x10000000)はflash.hで定義済み
//     const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
    
//     g_read_data[0] = flash_target_contents[0];
//     g_read_data[1] = flash_target_contents[1];
//     g_read_data[2] = flash_target_contents[2];
// }

// static void save_setting_to_flash(void)
// {
//     // W25Q16JVの最終ブロック(Block31)のセクタ0の先頭アドレス = 0x1F0000
//     const uint32_t FLASH_TARGET_OFFSET = 0x1F0000;
//     // W25Q16JVの書き込み最小単位 = FLASH_PAGE_SIZE(256Byte)
//     // FLASH_PAGE_SIZE(256Byte)はflash.hで定義済
//     uint8_t write_data[FLASH_PAGE_SIZE];

//     // 保存データのセット(例)
//     write_data[0] = 12;
//     write_data[1] = 34;
//     write_data[2] = 56;
    
//     // 割り込み無効にする
//     uint32_t ints = save_and_disable_interrupts();
//     // Flash消去。
//     //  消去単位はflash.hで定義されている FLASH_SECTOR_SIZE(4096Byte) の倍数とする
//     flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
//     // Flash書き込み。
//     //  書込単位はflash.hで定義されている FLASH_PAGE_SIZE(256Byte) の倍数とする
//     flash_range_program(FLASH_TARGET_OFFSET, write_data, FLASH_PAGE_SIZE);
//     // 割り込みフラグを戻す
//     restore_interrupts(ints);
// }
//  int main(){
//     stdio_init_all();
//     try
//     {
//         save_setting_to_flash();
//         sleep_ms(1000);
//         load_setting_from_flash();
//         for(;;){
//         printf("%d %d %d",g_read_data[0],g_read_data[1],g_read_data[2]);
//         sleep_ms(100);
//         }
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr << e.what() << '\n';
//     }
    
//     while (1) printf("ok");
    

//     return 0;
// }   


}

#endif  // SC19_PICO_SC_FLUSH_HPP_