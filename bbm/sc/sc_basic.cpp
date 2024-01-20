/**************************************************
 * SC19のプロジェクト全体にかかわるコードです
 * このファイルは，sc_basic.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，エラーや待機，ログ出力などに関するコードが定義されています．
**************************************************/

//! @file sc_basic.cpp
//! @brief プログラム全体で共通の，基本的な機能

#include "sc_basic.hpp"

namespace sc
{

/***** class Error *****/

Error::Error(const std::string& FILE, int LINE, const std::string& message) noexcept:
    _message(message)
{
    try
    {
        const std::string output_message = "<<ERROR>>  FILE : " + std::string(FILE) + "  LINE : " + std::to_string(LINE) + "\n           MESSAGE : " + _message + "\n";  // 出力する形式に変形
        std::cerr << output_message << std::endl;  // cerrでエラーとして出力

        print(output_message);  // エラーをログデータに記録
    }
    catch (const std::exception& e) {std::cerr << "<<ERROR>>  FILE : " << __FILE__ << "  LINE : " << __LINE__ << "/n           MESSAGE : Error logging failed.   " << e.what() << std::endl;}  // エラー：エラーログの記録に失敗しました
    catch(...) {std::cerr << "<<ERROR>>  FILE : " << __FILE__ << "  LINE : " << __LINE__ << "/n           MESSAGE : Error logging failed." << std::endl;}  // エラー：エラーログの記録に失敗しました
}

Error::Error(const std::string& FILE, int LINE, const std::exception& e, const std::string& message) noexcept:
    Error(FILE, LINE, message + "   " + e.what()) {}

const char* Error::what() const noexcept
{
    return _message.c_str();
}


/***** print *****/

void print(const std::string& message) noexcept
{
    try
    {
        std::cout << message << std::flush;
    }
    catch(const std::exception& e) {Error(__FILE__, __LINE__, "Failed to save log", e);}  // ログの保存に失敗しました
    catch(...) {Error(__FILE__, __LINE__, "Failed to save log");}  // ログの保存に失敗しました
}

}