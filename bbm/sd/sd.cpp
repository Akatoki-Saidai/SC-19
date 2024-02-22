#include "sd.hpp"

namespace sc 
{

SD::SD() try
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    pSD = sd_get_by_num(0);

    fr = f_mount(&pSD->fatfs, pSD->pcName, 1);

    if (FR_OK != fr)
    {
        SD::save = false;
        print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
        print(f_err(__FILE__, __LINE__, "f_mount error: %s (%d)\n", FRESULT_str(fr), fr));
    }
}
catch(const std::exception& e)
{
    SD::save = false;
    print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
    print(e.what());
}

SD::~SD()
{
    f_unmount(pSD->pcName);
}

void SD::write(const std::string& write_str)
{
    #ifdef DEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (SD::save == false)
    {
        throw std::runtime_error(f_err(__FILE__, __LINE__, "SD card is not working properly"));  // SDカードは正常に動作していません
    }

    FIL fil;
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
    {
        SD::save = false;
        print(f_err(__FILE__, __LINE__, "f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr));
        return;
    }
    if (f_printf(&fil, write_str.c_str()) < 0) {
        SD::save = false;
        printf("f_printf failed\n");
        return;
    }
    fr = f_close(&fil);
    if (FR_OK != fr) {
        SD::save = false;
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
        return;
    }
}

}