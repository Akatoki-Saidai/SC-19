#ifndef SC19_PICO_SC_BINARY_HPP_
#define SC19_PICO_SC_BINARY_HPP_

/**************************************************
 * 通信などで利用する，バイナリデータ(バイト列のデータ)を扱うためのコードです
 * このファイルは，binary.cppに書かれている関数の一覧です
 * 
 * C++には，配列を関数の戻り値にできない，配列を関数の引数にするのが難しいという特徴があります．
 * そのため，通信をする際にuint8_t型の配列を使うのではなく，このBinary型を使うことで送受信を簡単にしています．
**************************************************/

//! @file binary.hpp
//! @brief バイナリデータ(バイト列のデータ)を扱うためのコード

#include "sc_basic.hpp"

#include <string>
#include <vector>

namespace sc
{

class Binary;

//! @brief Binaryのデータをcoutで出力
std::ostream& operator<< (std::ostream&, const Binary&);
// この関数を作成するにあたり，以下を参考にしました
// https://programming-place.net/ppp/contents/cpp/language/035.html#to_stream


//! @brief 通信用のバイト列．
//! @note uint8_t型の配列では扱いにくいので代わりにこのクラスを使用します
class Binary
{
    const std::basic_string<uint8_t> _binary_data;  // バイト列のデータ
    friend std::ostream& operator<<(std::ostream& os, const Binary& binary);  // coutで出力できるように，<<演算子から_binary_dataへのアクセスを許可している
public:

    //! @brief 配列やvectorなどのコンテナからバイト列を作成
    //! @param data コンテナ形式のデータ
    template<typename Iterable, typename std::enable_if<IsIterable<Iterable>::value, std::nullptr_t>::type = nullptr>  // コンテナ形式の型でなければエラーになる
    Binary(const Iterable& data) try :
        _binary_data(std::begin(data), std::end(data))
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
    }
    catch(const std::exception& e)
    {
        sc::print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
        sc::print(e.what());
    }

    //! @brief { }からバイト列を作成
    //! @param init_list {1, 2, 3}などのデータ
    Binary(const std::initializer_list<uint8_t>& init_list) try :
        _binary_data(init_list) 
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
    }
    catch(const std::exception& e)
    {
        sc::print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
        sc::print(e.what());
    }

    //! @brief 配列からバイト列を作成
    //! @param array_ptr 配列
    //! @param size 配列の長さ
    template<class UINT8>
    Binary(const UINT8* array_ptr, std::size_t size) try :
        _binary_data(array_ptr, array_ptr + size) 
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
    }
    catch(const std::exception& e)
    {
        sc::print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
        sc::print(e.what());
    }

    //! @brief 1バイトの値からバイト列を作成
    Binary(const uint8_t& bite_data) try :
        _binary_data({bite_data}) 
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
    }
    catch(const std::exception& e)
    {
        sc::print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n\n********************\n", __FILE__, __LINE__);
        sc::print(e.what());
    }

    //! @brief バイト列のサイズを返す
    //! @return バイト列のサイズ
    std::size_t size() const;

    //! @brief バイト列のindex番目の値を返す
    //! @param index 先頭から何番目か．先頭は0
    //! @return index番目の値．
    //! @note バイト列のサイズ以上のindexを指定した場合，エラーstd::out_of_rangeを返す．
    uint8_t at(std::size_t index) const;

    //! @brief string型に変換
    //! @return string型のデータ
    std::string str() const;

    //! @brief バイト列のindex番目の値を返す
    //! @param index 先頭から何番目か．先頭は0
    //! @return index番目の値．
    //! @note バイト列のサイズ以上のindexを指定した場合，エラーが発生するかは未定義．
    uint8_t operator[](std::size_t index) const;

    //! @brief バイト列のindex番目の値を返す
    //! @param index 先頭から何番目か．先頭は0
    //! @return index番目の値．
    //! @note バイト列のサイズ以上のindexを指定した場合，エラーが発生するかは未定義．
    uint8_t operator[](int index) const 
        {return _binary_data[index];}

    //! @brief vector型への変換
    //! @return vector型の値
    operator std::vector<uint8_t>() const;

    //! @brief string型への変換
    //! @return string型の値
    operator std::string() const;

    //! @brief uint8_t型の配列に変換
    //! @note 末尾に'\0'が付加されます．このポインタを通して値を変更した場合の動作は未定義です．
    operator const uint8_t*() const;

    //! @brief データの先頭へのアドレスを返す
    //! @note 末尾に'\0'が付加されます．このポインタを通して値を変更した場合の動作は未定義です．
    const uint8_t* operator&() const;
    
    //! @brief バイナリデータを結合
    Binary operator+ (const Binary& other_binary) const;

    //! @brief 配列の先頭へのポインタにデータを代入
    void to_assign(uint8_t* reg_ptr) const;

    //! @brief 生データにアクセス
    const std::basic_string<uint8_t>& data() const
        {return _binary_data;}
};

//! @brief バイナリデータの先頭に1バイト追加
Binary operator+ (uint8_t first_byte, const Binary& binary);

//! @brief バイナリデータの末尾に1バイト追加
Binary operator+ (const Binary& binary, uint8_t end_byte);

}

#endif  // SC19_PICO_SC_BINARY_HPP_