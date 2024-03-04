#ifndef SC19_PICO_TWELITE_HPP_
#define SC19_PICO_TWELITE_HPP_

#include "sc.hpp"

#include <algorithm>

namespace sc 
{

class Twelite 
{
    const UART& _uart;
public:
    Twelite(const UART& uart) try :
        _uart(uart) 
    {            
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        try
        {
        }
        catch(const std::exception& e)
        {
            print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
        }
    }
    catch (const std::exception& e)
    {
        print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
    }

    // App_Twelite(超簡単！標準アプリ)は以下の仕様で通信します
    // https://mono-wireless.com/jp/products/TWE-APPS/App_Twelite/step3-01.html

    //! @brief TWELITEの「超簡単！標準アプリ」の書式でUART出力します
    //! @param device_id 送信先のデバイスID (親機は0x00)
    //! @param binary 送信するデータ
    void write(uint8_t device_id, const Binary& binary);

    //! @brief TWELITEの「超簡単！標準アプリ」の書式でUARTを受け取ります．
    // チェックサムは未実装
    Binary read();
};

}


#endif  // SC19_PICO_TWELITE_HPP_