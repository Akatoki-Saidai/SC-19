#include "hardware/flash.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"

uint8_t g_read_data[3];

void load_setting_from_flash(void)
{
    // W25Q16JVの最終ブロック(Block31)のセクタ0の先頭アドレス = 0x1F0000
    const uint32_t FLASH_TARGET_OFFSET = 0x1F0000;
    // XIP_BASE(0x10000000)はflash.hで定義済み
    const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
    
    g_read_data[0] = flash_target_contents[0];
    g_read_data[1] = flash_target_contents[1];
    g_read_data[2] = flash_target_contents[2];
}
int main(){
    load_setting_from_flash();
    return 0;
}