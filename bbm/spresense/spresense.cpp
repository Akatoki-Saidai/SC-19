
/**************************************************
 * Spresenseとの通信に関するコードです
 * このファイルは，spresense.hppに名前だけ書かれている関数の中身です
 * 
**************************************************/

//! @file spresense.cpp
//! @brief Spresenseとの通信

#include "spresense.hpp"

namespace sc
{

void Spresense::read()
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    const Binary& read_binary = _uart.read();
    std::size_t index = 0;
    while (index < read_binary.size())
    {
        int start_index = read_binary.data().find(':', index);
        int end_index = read_binary.data().find('\n', start_index);
        if (start_index==-1 || end_index==-1)
    break;
        std::string head = std::string(read_binary.data().begin()+start_index, read_binary.data().begin()+start_index+4);
        double number = stod(std::string(read_binary.data().begin()+start_index+4, read_binary.data().begin()+end_index));
        if (head == ":Lat")
        {
            _lat = number;
            _lat_update = get_absolute_time();
        } else if (head == ":Lon")
        {
            _lon = number;
            _lon_update = get_absolute_time();
        } else if (head == ":Cam")
        {
            _cam = Cam(int(number));
            _cam_update = get_absolute_time();
        } else if (head == ":Tim")
        {
            auto num_itr = read_binary.data().begin()+start_index+4;
            if ((end_index-start_index) == 16 || (end_index-start_index) == 18)
            {
                _time.tm_year = stoi(std::string(num_itr, num_itr+4)) - 1900;  // 1900からの経過年数
                _time.tm_mon = stoi(std::string(num_itr+4, num_itr+6)) - 1;  // 0 = 1月
                _time.tm_mday = stoi(std::string(num_itr+6, num_itr+8));  // 日
                _time.tm_hour = stoi(std::string(num_itr+8, num_itr+10));  // 時
                _time.tm_min = stoi(std::string(num_itr+10, num_itr+12));  // 分
                if ((end_index-start_index) == 18)
                    _time.tm_sec = stoi(std::string(num_itr+12, num_itr+14));  // 秒
                _time_update = get_absolute_time();
            }
        }
        // 以下の資料を参考にしました
        // https://qiita.com/grapefruit1030/items/900571bf10dea8740cd7

        index = end_index + 1;
    }
}


std::tuple<Latitude<Unit::deg>, Longitude<Unit::deg> > Spresense::gps()
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (absolute_time_diff_us(_lat_update, _start_time) == 0 || absolute_time_diff_us(_lon_update, _start_time) == 0)
        read();
    if (absolute_time_diff_us(_lat_update, get_absolute_time()) > 5*1000*1000 || absolute_time_diff_us(_lon_update, get_absolute_time()) > 5*1000*1000)
        read();
    if (absolute_time_diff_us(_lat_update, get_absolute_time()) > 10*1000*1000 || absolute_time_diff_us(_lon_update, get_absolute_time()) > 10*1000*1000)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Latest GPS data not available"));  // 最新のGPSのデータがありません
    }
    return std::tuple(Latitude<Unit::deg>(_lat), Longitude<Unit::deg>(_lon));
}


Cam Spresense::camera()
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (absolute_time_diff_us(_cam_update, _start_time) == 0)
        read();
    if (absolute_time_diff_us(_cam_update, get_absolute_time()) > 5*1000*1000)
        read();
    if (absolute_time_diff_us(_cam_update, get_absolute_time()) > 5*1000*1000)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Latest camera data not available"));  // 最新のカメラのデータがありません
    }
    return _cam;
}


std::tm Spresense::time()
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (absolute_time_diff_us(_time_update, _start_time) == 0)
        read();
    if (absolute_time_diff_us(_time_update, _start_time) == 0)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Latest time data not available"));  // 最新の時刻のデータがありません
    }
    time_t new_time = mktime(&_time) + absolute_time_diff_us(_time_update, get_absolute_time())/(1000*1000);
    _new_time = *gmtime(&new_time);
    return _new_time;
}


}