#include "sd.hpp"

namespace sc 
{

SD::SD(MISO miso, MOSI mosi, SCK sck, Pin ss, Pin card_detect, dimension::Hz freq)
{
    if (!(miso.get_spi_id() == mosi.get_spi_id() && mosi.get_spi_id() == sck.get_spi_id()))
    {
        throw Error(__FILE__, __LINE__, "An incorrect SPI pin number was entered");  // 正しくないSPIのピン番号が入力されました
    }

    *spi_get_by_num(0) = 
    {
        .hw_inst = (miso.get_spi_id() ? spi1 : spi0),
        .miso_gpio = miso.gpio(),
        .mosi_gpio = mosi.gpio(),
        .sck_gpio = sck.gpio(),
        .baud_rate = static_cast<unsigned int>(double(freq))
    };

    *sd_get_by_num(0) = 
    {
        .pcName = "0:",   // Name used to mount device
        .spi = spi_get_by_num(0),  // Pointer to the SPI driving this card
        .ss_gpio = ss.gpio(),    // The SPI slave select GPIO for this SD card
        .use_card_detect = true,
        .card_detect_gpio = card_detect.gpio(),   // Card detect
        .card_detected_true = 1
    };

    pSD = sd_get_by_num(0);

    fr = f_mount(&pSD->fatfs, pSD->pcName, 1);

    if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
}

SD::~SD()
{
    f_unmount(pSD->pcName);
}

void SD::write(std::string& write_str)
{
    FIL fil;
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    if (f_printf(&fil, write_str.c_str()) < 0) {
        printf("f_printf failed\n");
    }
    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
}

}