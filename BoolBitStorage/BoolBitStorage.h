//
// Created by Jeremie on 2020/03/25.
//

#ifndef H_BOOLBITSTORAGE_H
#define H_BOOLBITSTORAGE_H

#include <Arduino.h>


typedef uint8_t BoolBit8;
typedef uint16_t BoolBit16;
typedef uint32_t BoolBit32;

template<typename T>
uint8_t getBoolBitP(T *data, uint8_t i) {
    return ((*data & (0x1u << i)) ? 0x1u : 0x0u);
}

template<typename T>
void setBoolBitP(T *data, uint8_t i, uint8_t b) {
    T flag = (0x1u << i);
    if (b) {
        *data |= flag;
    } else {
        flag = ~flag;
        *data &= flag;
    }
}

#define setBoolBit(data, i, b) setBoolBitP(&data, i, b)
#define getBoolBit(data, i) getBoolBitP(&data, i)


#endif //H_BOOLBITSTORAGE_H
