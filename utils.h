//
// Created by tapanthaker on 6/21/20.
//

#ifndef YLD_UTILS_H
#define YLD_UTILS_H

#include <iostream>
#include <iomanip>

template< typename T >
std::string int_to_hex( T i ){
    std::stringstream stream;
    stream << "0x"
           << std::setfill ('0') << std::setw(sizeof(T)*2)
           << std::hex << i;
    return stream.str();
}

#endif //YLD_UTILS_H
