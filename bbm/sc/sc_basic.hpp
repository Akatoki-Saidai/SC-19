#ifndef SC19_PICO_SC_SC_BASIC_HPP_
#define SC19_PICO_SC_SC_BASIC_HPP_

/**************************************************
 * SC19のプロジェクト全体にかかわるコードです
 * このファイルは，sc_basic.cppに書かれている関数の一覧です
 * 
 * このファイルでは，エラーや待機，ログ出力などに関するコードが宣言されています．
**************************************************/

//! @file sc_basic.hpp
//! @brief プログラム全体で共通の，基本的な機能

#include "pico/stdlib.h"

#define _USE_MATH_DEFINES  // 円周率などの定数を使用する  math.hを読み込む前に定義する必要がある (math.hはcmathやiostreamに含まれる)
#include <algorithm>
#include <cfloat>  // double型の最小値など
#include <iostream>  // coutなど
#include <string>

#include "unit.hpp"
#include "pin.hpp"

namespace sc
{

template<typename... Args> std::string format_str(const std::string&, Args...) noexcept;

//! @brief エラーを記録し保持
//! エラーを記録します．例外としてこのクラスを投げることができます．
//! @note エラーを発生させる場合は次のようにしてください
//!     throw sc::Error(__FILE__, __LINE__, "error message");
class Error : public std::exception
{
    const std::string _message;  // エラーメッセージ
public:
    //! @brief エラーを記録し，標準エラー出力に出力します
    //! @param FILE __FILE__としてください (自動でファイル名に置き換わります)
    //! @param LINE __LINE__としてください (自動で行番号に置き換わります)
    //! @param message 出力したいエラーメッセージ (自動で改行)
    Error(const std::string& FILE, int LINE, const std::string& message) noexcept;

    //! @brief キャッチしたエラーを記録し，標準エラー出力に出力します
    //! @param FILE ＿FILE＿としてください (自動でファイル名に置き換わります)
    //! @param LINE ＿LINE＿としてください (自動で行番号に置き換わります)
    //! @param e キャッチした例外
    //! @param message 出力したいエラーメッセージ (自動で改行)
    Error(const std::string& FILE, int LINE, const std::exception& e, const std::string& message) noexcept;

    //! @brief エラーを記録し，printfの形式で標準エラー出力に出力します
    //! @param FILE __FILE__としてください (自動でファイル名に置き換わります)
    //! @param LINE __LINE__としてください (自動で行番号に置き換わります)
    //! @param format フォーマット文字列
    //! @param args フォーマット文字列に埋め込む値
    template<typename... Args>
    Error(const std::string& FILE, int LINE, const std::string& format, Args... args) noexcept:
        Error(FILE, LINE, format_str(format, args...)) {}

    //! @brief エラーについての説明文を返します
    //! @return エラーの説明
    const char* what() const noexcept override;
};


//! @brief printfの形式で文字列をフォーマット
//! @param format フォーマット文字列
//! @param args フォーマット文字列に埋め込む値
template<typename... Args>
std::string format_str(const std::string& format, Args... args) noexcept
{
    try
    {
        const size_t formatted_chars_num = std::snprintf(nullptr, 0, format.c_str(), args...);  // フォーマット後の文字数を計算
        char formatted_chars[formatted_chars_num + 1];  // フォーマット済みの文字列を保存するための配列を作成
        std::snprintf(&formatted_chars[0], formatted_chars_num + 1, format.c_str(), args...);  // フォーマットを実行
return std::string(formatted_chars);  // フォーマット済み文字列を出力
    }
    catch(const std::exception& e) {Error(__FILE__, __LINE__, e, "Failed to format string");}  // ログの保存に失敗しました
    catch(...) {Error(__FILE__, __LINE__, "Failed to format string");}  // ログの保存に失敗しました
    return "ERROR";
}
// この関数は以下の資料を参考にて作成しました
// https://pyopyopyo.hatenablog.com/entry/2019/02/08/102456


//! @brief メッセージを出力する関数です．
//! @param message 出力する文字列
//! 出力時にSDカードにも記録する場合などは，この関数に追記してください
void print(const std::string& message) noexcept;

//! @brief printfの形式で出力
//! @param format フォーマット文字列
//! @param args フォーマット文字列に埋め込む値
template<typename... Args>
void print(const std::string& format, Args... args) noexcept
{
    print(format_str(format, args...));
}


//! @brief 指定した時間 待機
//! @param time 待機する時間 (100_ms のように入力)
inline void sleep(_ms time)
{
    ::sleep_us(static_cast<double>(static_cast<_us>(time)));  // pico-sdkの関数  マイクロ秒待機
}


//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す
//! @param value 0かもしれない数
//! @return 0ではない数
template<typename T> inline T not0(T value) {return (value ? value : 1);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．floatバージョン
//! @param value 0かもしれない数
template<> inline float not0(float value) {return (value ? value : FLT_TRUE_MIN);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．doubleバージョン
//! @param value 0かもしれない数
//! @return 0ではない数
template<> inline double not0(double value) {return (value ? value : DBL_TRUE_MIN);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．long doubleバージョン
//! @param value 0かもしれない数
//! @return 0ではない数
template<> inline long double not0(long double value) {return (value ? value : LDBL_TRUE_MIN);}


//! @brief コピーを禁止するための親クラス
class Noncopyable
{
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};
// Noncopyableクラスは以下の資料を参考にして作成しました
// https://cpp.aquariuscode.com/uncopyable-mixin

}


// //! @brief 配列がある値を含んでいるかを調べる
// //! @param one 比較する値
// //! @param list 中身を調べる配列
// //! @return 配列listの要素が一つでもoneと等しかったらtrue，いずれも等しくなかったらfalse
// template<typename T, typename U>
// constexpr bool any_of(T&& one, U&& list)
// {
//     for (auto element : list)
//     {
//         if (one == element)
// return true;
//     }
//     return false;
// }

//! @brief 配列がある値を含んでいるかを調べる
template<typename T>
struct any_of
{
    const T& _compare_list;

    //! @brief 配列がある値を含んでいるかを調べる
    //! @param list 中身を比べる配列
    constexpr any_of(const T& list):
        _compare_list(list) {}
};

//! @brief 配列がある値を含んでいるかを調べる
//! @param one 比較する値
//! @param list 比較する配列
template<typename T, typename U>
constexpr bool operator==(const T& one, const any_of<U>& list)
{
    for (auto element : list._compare_list)
    {
        if (one == element)
return true;
    }
    return false;
}


//! @brief 配列がある値を含んでいないことを調べる
template<typename T>
struct all_of
{
    const T& _compare_list;

    //! @brief 配列がある値を含んでいないことを調べる
    //! @param list 中身を比べる配列
    constexpr all_of(const T& list):
        _compare_list(list) {}
};

//! @brief 配列がある値を含んでいないことを調べる
//! @param one 比較する値
//! @param list 比較する配列
template<typename T, typename U>
constexpr bool operator!=(const T& one, const all_of<U>& list)
{
    for (auto element : list._compare_list)
    {
        if (one == element)
return false;
    }
    return true;
}

#endif  // SC19_PICO_SC_SC_BASIC_HPP_