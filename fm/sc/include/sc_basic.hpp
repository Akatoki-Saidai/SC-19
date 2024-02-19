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
#include <cstddef>  // size_tなど
#include <functional>  // function
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
    //! @brief エラーを記録します
    //! @param FILE __FILE__としてください (自動でファイル名に置き換わります)
    //! @param LINE __LINE__としてください (自動で行番号に置き換わります)
    //! @param message 出力したいエラーメッセージ (自動で改行)
    Error(const std::string& FILE, int LINE, const std::string& message) noexcept:
        _message("<<ERROR>>  FILE : " + std::string(FILE) + "  LINE : " + std::to_string(LINE) + "\n           MESSAGE : " + _message + "\n") 
    {
        printf(_message.c_str());
        sleep_ms(100);
    }

    //! @brief キャッチしたエラーを記録ます
    //! @param FILE ＿FILE＿としてください (自動でファイル名に置き換わります)
    //! @param LINE ＿LINE＿としてください (自動で行番号に置き換わります)
    //! @param e キャッチした例外
    //! @param message 出力したいエラーメッセージ (自動で改行)
    Error(const std::string& FILE, int LINE, const std::exception& e, const std::string& message) noexcept:
        _message(std::string(e.what()) + "required from here\n" + "<<ERROR>>  FILE : " + std::string(FILE) + "  LINE : " + std::to_string(LINE) + "\n           MESSAGE : " + _message + "\n") 
    {
        printf(_message.c_str());
        sleep_ms(100);
    }

    //! @brief printfの形式でエラーメッセージを記録します
    //! @param FILE __FILE__としてください (自動でファイル名に置き換わります)
    //! @param LINE __LINE__としてください (自動で行番号に置き換わります)
    //! @param format フォーマット文字列
    //! @param args フォーマット文字列に埋め込む値
    template<typename... Args>
    Error(const std::string& FILE, int LINE, const std::string& format, Args... args) noexcept:
        Error(FILE, LINE, format_str(format, args...)) {}

    //! @brief エラーについての説明文を返します
    //! @return エラーの説明
    const char* what() const noexcept override
        {return _message.c_str();}
};


//! @brief printfの形式で文字列をフォーマット
//! @param format フォーマット文字列
//! @param args フォーマット文字列に埋め込む値
template<typename... Args>
std::string format_str(const std::string& format, Args... args) noexcept
{
    try
    {
        const std::size_t formatted_chars_num = std::snprintf(nullptr, 0, format.c_str(), args...);  // フォーマット後の文字数を計算
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


//! @brief メッセージを出力する関数．
//! @note SDカードなどに出力するときは上書きしてください
//! @param message 出力する文字列
inline std::function<void(const std::string&)> set_print = [](const std::string& message)
{
    try
    {
        std::cout << message << std::flush;
    }
    catch(const std::exception& e) {printf("Failed to save log\n");}  // ログの保存に失敗しました
};

//! @brief printfの形式で出力
//! @param format フォーマット文字列
//! @param args フォーマット文字列に埋め込む値
template<typename... Args>
void print(const std::string& format, Args... args)
{
    set_print(format_str(format, args...));
}

//! @brief エラーメッセージを出力
//! @param e キャッチしたエラー
inline void print(const std::exception& e)
{
    set_print(e.what());
}


//! @brief 指定した時間 待機
//! @param time 待機する時間 (100_ms のように入力)
inline void sleep(Time<Unit::s> time)
{
    ::sleep_us(static_cast<double>(time * (1/micro)));  // pico-sdkの関数  マイクロ秒待機
}


//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す
//! @param value 0かもしれない数
//! @return 0ではない数
template<typename T> constexpr inline T not0(const T& value) {return (value ? value : 1);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．floatバージョン
//! @param value 0かもしれない数
template<> constexpr inline float not0(const float& value) {return (value ? value : FLT_TRUE_MIN);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．doubleバージョン
//! @param value 0かもしれない数
//! @return 0ではない数
template<> constexpr inline double not0(const double& value) {return (value ? value : DBL_TRUE_MIN);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．long doubleバージョン
//! @param value 0かもしれない数
//! @return 0ではない数
template<> constexpr inline long double not0(const long double& value) {return (value ? value : LDBL_TRUE_MIN);}



//! @brief エラー防止のため，-0.0であるかを判定し+0.0を返す．floatバージョン
//! @param value -0.0かもしれない数
//! @return -0.0ではない数
constexpr inline float not_minus0(const float& value) {return (value ? value : 0.0F);}

//! @brief エラー防止のため，-0.0であるかを判定し+0.0を返す．doubleバージョン
//! @param value -0.0かもしれない数
//! @return -0.0ではない数
constexpr inline double not_minus0(const double& value) {return (value ? value : 0.0);}

//! @brief エラー防止のため，-0.0であるかを判定し+0.0を返す．long doubleバージョン
//! @param value -0.0かもしれない数
//! @return -0.0ではない数
constexpr inline long double not_minus0(const long double& value) {return (value ? value : 0.0L);}


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






//! @brief 配列やコンテナであるかを調べる
template <typename T>
class IsIterable
{
private:
    template <typename U>  // カンマ演算子を利用している
    static constexpr auto check1(U&& u) -> decltype(&U::begin, &U::end, std::true_type());  // メンバ関数begin()とend()を持つならtrue
    static constexpr std::false_type check1(...);

    template <typename U>
    static constexpr auto check2(U&& u) -> decltype(std::begin(u), std::end(u), std::true_type());  // std::begin(u)とstd::end(u)が存在するならtrue
    static constexpr std::false_type check2(...);
    
    template <typename U>
    static constexpr auto check3(U&& u) -> decltype(begin(u), end(u), std::true_type());  // begin(u)とend(u)が存在するならtrue
    static constexpr std::false_type check3(...);
public:
    //! @brief beginとendを持っていたらtrue，持っていなかったらfalse
    static constexpr bool value = (decltype(check1(std::declval<T>()))::value || decltype(check2(std::declval<T>()))::value || decltype(check3(std::declval<T>()))::value);
};
// このクラスは以下の資料を参考にし作成しました
// https://qiita.com/terukazu/items/e257c05a7b191d32c577
// https://zenn.dev/ymd_h/articles/e90ad8ad40a6dd


//! @brief 配列がある値を含んでいるかを調べる
template<typename T>
struct any_of
{
    const T& _compare_list;

    //! @brief 配列がある値を含んでいるかを調べる
    //! @param list 中身を比べる配列
    template<typename Iterable>
    constexpr any_of(const Iterable& list):
        _compare_list(list) {}
};
// 推論補助宣言
template<typename Iterable>
any_of(const Iterable&)
    -> any_of<typename std::enable_if<IsIterable<Iterable>::value, Iterable>::type >;  // コンテナ以外が引数に渡されたらエラーになる

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
    template<typename Iterable>
    constexpr all_of(const Iterable& list):
        _compare_list(list) {}
};
// 推論補助宣言
template<typename Iterable>
all_of(const Iterable&)
    -> all_of<typename std::enable_if<IsIterable<Iterable>::value, Iterable>::type >;  // コンテナ以外が引数に渡されたらエラーになる

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


}

#endif  // SC19_PICO_SC_SC_BASIC_HPP_