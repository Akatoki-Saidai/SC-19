#include "hardware/flash.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"

static void save_setting_to_flash(void)
{
    // W25Q16JVの最終ブロック(Block31)のセクタ0の先頭アドレス = 0x1F0000
    const uint32_t FLASH_TARGET_OFFSET = 0x1F0000;
    // W25Q16JVの書き込み最小単位 = FLASH_PAGE_SIZE(256Byte)
    // FLASH_PAGE_SIZE(256Byte)はflash.hで定義済
    uint8_t write_data[FLASH_PAGE_SIZE];

    // 保存データのセット(例)
    write_data[0] = 12;
    write_data[1] = 34;
    write_data[2] = 56;
    
    // 割り込み無効にする
    uint32_t ints = save_and_disable_interrupts();
    // Flash消去。
    //  消去単位はflash.hで定義されている FLASH_SECTOR_SIZE(4096Byte) の倍数とする
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    // Flash書き込み。
    //  書込単位はflash.hで定義されている FLASH_PAGE_SIZE(256Byte) の倍数とする
    flash_range_program(FLASH_TARGET_OFFSET, write_data, FLASH_PAGE_SIZE);
    // 割り込みフラグを戻す
    restore_interrupts(ints);
}
 int main(){
    save_setting_to_flash();
    return 0;
}   

