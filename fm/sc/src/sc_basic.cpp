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