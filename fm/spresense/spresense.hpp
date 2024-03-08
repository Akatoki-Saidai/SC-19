#ifndef SC19_PICO_SPRESENSE_HPP_
#define SC19_PICO_SPRESENSE_HPP_

#include "sc.hpp"

#include <cstdlib>
#include <ctime>
#include <string>
#include <tuple>

namespace sc 
{

    
enum class Cam
{
    Left = 1,
    Center,
    Right,
    NotFound,
    Reset
};

class Spresense 
{
    const UART& _uart;  // 通信に使用するUART

    const absolute_time_t _start_time = get_absolute_time();

    std::tm _time = {0};  // 受信した時刻
    std::tm _new_time;  // 計算後の一時的な時刻(ポインタ関連でメンバ変数にする必要がある)
    absolute_time_t _time_update = _start_time;  // 受信した時刻

    Cam _cam;
    absolute_time_t _cam_update = _start_time;  // 受信した時刻

    double _lat = 0.0;
    absolute_time_t _lat_update = _start_time;  // 受信した時刻

    double _lon = 0.0;
    absolute_time_t _lon_update = _start_time;  // 受信した時刻

    std::basic_string<uint8_t> _read_binary = {};  // 受信したデータ
    static constexpr std::size_t _max_size = 100;  // 受信したデータを保存しておく最大のバイト数

    void read();

public:
    Spresense(const UART& uart) try :
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

    std::tuple<Latitude<Unit::deg>, Longitude<Unit::deg> > gps();

    Cam camera();

    //! @brief 現在の世界協定時を返す
    std::tm time();
};

}


#endif  // SC19_PICO_SPRESENSE_HPP_