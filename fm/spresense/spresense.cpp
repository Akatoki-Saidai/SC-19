
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
    _read_binary += _uart.read().data();
    std::size_t index = 0;
    while (index < _read_binary.size())
    {
        int start_index = _read_binary.find(':', index);
        int end_index = _read_binary.find('\n', start_index);
        if (start_index<0 || end_index<0)
    return;
        std::string head = std::string(_read_binary.begin()+start_index, _read_binary.begin()+start_index+4);
        double number = stod(std::string(_read_binary.begin()+start_index+4, _read_binary.begin()+end_index));
        if (head == ":Lat")
        {
            if (number != 0.0)
            {
                _lat = number;
                _lat_update = get_absolute_time();
            }
        } else if (head == ":Lon")
        {
            if (number != 0.0)
            {
                _lon = number;
                _lon_update = get_absolute_time();
            }
        } else if (head == ":Cam")
        {
            if (Cam(int(number)) == Cam::Left || Cam(int(number)) == Cam::Center || Cam(int(number)) == Cam::Right)
            {
                _cam = Cam(int(number));
                _cam_update = get_absolute_time();
            }
        } else if (head == ":Tim")
        {
            std::tm new_time;
            auto num_itr = _read_binary.begin()+start_index+4;
            if ((end_index-start_index) == 19)
            {
                new_time.tm_year = stoi(std::string(num_itr, num_itr+4)) - 1900;  // 1900からの経過年数
                new_time.tm_mon = stoi(std::string(num_itr+4, num_itr+6)) - 1;  // 0 = 1月
                new_time.tm_mday = stoi(std::string(num_itr+6, num_itr+8));  // 日
                new_time.tm_hour = stoi(std::string(num_itr+8, num_itr+10));  // 時
                new_time.tm_min = stoi(std::string(num_itr+10, num_itr+12));  // 分
                new_time.tm_sec = stoi(std::string(num_itr+12, num_itr+14));  // 秒
                if (2020 < new_time.tm_year && new_time.tm_year < 2050)
                {
                    _time = new_time;
                    _time_update = get_absolute_time();
                }
            }
        }
        // 以下の資料を参考にしました
        // https://qiita.com/grapefruit1030/items/900571bf10dea8740cd7

        index = end_index + 1;
    }
    if (_read_binary.size() > _max_size)
    {
        _read_binary.replace(0, _read_binary.size() - _max_size, {});
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
    if (absolute_time_diff_us(_lat_update, get_absolute_time()) > 10*1000*1000 || absolute_time_diff_us(_lon_update, get_absolute_time()) > 10*1000*1000 || absolute_time_diff_us(_lat_update, _start_time) == 0 || absolute_time_diff_us(_lon_update, _start_time) == 0)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Latest GPS data not available"));  // 最新のGPSのデータがありません
    }
    print("gps_read_data:%f,%f\n", _lat, _lon);
    return std::tuple(Latitude<Unit::deg>(_lat), Longitude<Unit::deg>(_lon));
}


Cam Spresense::camera()
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    _uart.write("CameraStart\n");
    if (absolute_time_diff_us(_cam_update, _start_time) == 0)
        read();
    if (absolute_time_diff_us(_cam_update, get_absolute_time()) > 5*1000*1000)
        read();
    if (absolute_time_diff_us(_cam_update, get_absolute_time()) > 5*1000*1000 || absolute_time_diff_us(_cam_update, _start_time) == 0)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Latest camera data not available"));  // 最新のカメラのデータがありません
    }
    print("camera_read_data:%d\n", int(_cam));
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

    std::string time_str(20, '\0');
    strftime(time_str.data(), 20, "%FT%TZ\n", &_new_time);
    print("time_stamp:%s", time_str.c_str());

    return _new_time;
}


}