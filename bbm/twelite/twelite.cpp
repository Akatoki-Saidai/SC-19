
/**************************************************
 * TWELITEの「超簡単！標準アプリ」でのUART通信に関するコードです
 * このファイルは，twelite.hppに名前だけ書かれている関数の中身です
 * 
**************************************************/

//! @file twelite.cpp
//! @brief TWELITEの「超簡単！標準アプリ」でのUART通信

#include "twelite.hpp"

namespace sc
{

void Twelite::write(uint8_t device_id, const Binary& binary)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    constexpr uint8_t command = 0x01;

    // std::cout << "b:" << binary << std::endl;

    for (std::size_t i=0; i*80<=binary.size(); ++i)
    {
        std::basic_string<uint8_t> split_binary(binary.data().data()+(80*i), binary.data().data()+std::min(binary.size(), 80*(i+1)));
        // std::cout << "sb : " << split_binary.back() << std::endl;
    
        unsigned int bite_sum = (((unsigned int)device_id + command) & 0xff);
        for (uint8_t b : split_binary)
        {
            bite_sum = ((bite_sum + b) & 0xff);
        }
        const uint8_t check_sum = 0x100 - bite_sum;

        Binary new_binary = (device_id + (command + split_binary)) + check_sum;

        std::string converted_str(new_binary.size()*2+1, 0);
        converted_str.at(0) = ':';
        for (std::size_t j=0; j<new_binary.size(); ++j)
        {
            sprintf(converted_str.data()+(2*j)+1, "%02hhX", new_binary.at(j));
        }
        converted_str += "\r\n";
        // std::cout << converted_str << std::endl;
        _uart.write(converted_str);
    }
}


Binary Twelite::read()
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    const Binary& read_binary = _uart.read();
    std::basic_string<uint8_t> return_binary = {};
    std::size_t index = 0;
    while (index < read_binary.size())
    {
        int start_index = read_binary.data().find(':', index);
        // std::cout << "start:" << start_index << std::endl;
        int end_index = read_binary.data().find('\n', start_index);
        // std::cout << "end:" << end_index << std::endl;
        if (start_index<0 || end_index<0)
    return return_binary;
        // std::cout << "size:" << (end_index - start_index) << std::endl;
        if (end_index - start_index < 11)
    return return_binary;
        if ((end_index - start_index) %2 == 1)
    return return_binary;
        // 通信コマンドを確認
        if (read_binary.at(start_index+3)!='0' || read_binary.at(start_index+4)!='1')
    return return_binary;

        std::basic_string<uint8_t> new_binary((end_index - start_index - 8)/2, 0);
        for (int old_i = start_index+5, new_i=0; old_i<(end_index-3); old_i+=2, ++new_i)
        {
            unsigned int big8 = read_binary.at(old_i);
            unsigned int small8 = read_binary.at(old_i+1);
            // std::cout << "newi:" << new_i << " oldi:" << old_i << "  big:" << big8 << " small:" << small8 << std::endl;
            if ('0' <= big8 && big8 <= '9')
            {
                new_binary.at(new_i) = ((big8 - '0') << 4);
            } else if ('A' <= big8 && big8 <= 'F') {
                new_binary.at(new_i) = ((big8 - 'A' + 10) << 4);
            } else {
    return return_binary;
            }
            if ('0' <= small8 && small8 <= '9')
            {
                new_binary.at(new_i) += (small8 - '0');
            } else if ('A' <=small8 && small8 <= 'F') {
                new_binary.at(new_i) += (small8 - 'A' + 10);
            } else {
    return return_binary;
            }
            // std::cout << "new_at" << int(new_binary.at(new_i)) << std::endl;
        }

        return_binary += new_binary;

        index = end_index + 1;
    }
    return return_binary;
}

}