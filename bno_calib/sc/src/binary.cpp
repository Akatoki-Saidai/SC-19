/**************************************************
 * 通信などで利用する，バイナリデータ(バイト列のデータ)を扱うためのコードです
 * このファイルは，binary.hppに名前だけ書かれている関数の中身です
 * 
 * C++には，配列を関数の戻り値にできない，配列を関数の引数にするのが難しいという特徴があります．
 * そのため，通信をする際にuint8_t型の配列を使うのではなく，このBinary型を使うことで送受信を簡単にしています．
**************************************************/

//! @file binary.cpp
//! @brief バイナリデータ(バイト列のデータ)を扱うためのコード

#include "binary.hpp"

namespace sc
{

std::size_t Binary::size() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return _binary_data.size();
}

uint8_t Binary::at(std::size_t index) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return _binary_data.at(index);
}

std::string Binary::str() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return std::string(_binary_data.begin(), _binary_data.end());
}

uint8_t Binary::operator[](std::size_t index) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return _binary_data[index];
}

Binary::operator std::vector<uint8_t>() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return std::vector<uint8_t>(_binary_data.begin(), _binary_data.end());
}

Binary::operator std::string() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return std::string(_binary_data.begin(), _binary_data.end());
}

Binary::operator const uint8_t*() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return _binary_data.c_str();
}

const uint8_t* Binary::operator&() const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return _binary_data.c_str();
}

Binary Binary::operator+ (const Binary& other_binary) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return Binary(this->_binary_data + other_binary._binary_data);
}

void Binary::to_assign(uint8_t* reg_ptr) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    for (std::size_t i=0; i<_binary_data.size(); ++i)
    {
        *reg_ptr = _binary_data.at(i);
        ++reg_ptr;
    }
}

Binary operator+ (uint8_t first_byte, const Binary& binary)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return Binary{first_byte} + binary;
}

Binary operator+ (const Binary& binary, uint8_t end_byte)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    return binary + Binary{end_byte};
}


std::ostream& operator<<(std::ostream& os, const Binary& binary)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    os << binary._binary_data.c_str();
    return os;
}

}