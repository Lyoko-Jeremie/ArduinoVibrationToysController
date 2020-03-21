//
// Created by jeremie on 2020/03/22.
//


#ifndef TESTCLION_4X4MATRIXKEYBOARD_H
#define TESTCLION_4X4MATRIXKEYBOARD_H

void SetPin(
        int _row0,
        int _row1,
        int _row2,
        int _row3,
        int _col0,
        int _col1,
        int _col2,
        int _col3
);

void initPortState();

void ScanKeyAndCallKeyCallBackFunction();

uint8_t GetKeyState(uint8_t i);

typedef void (CallBack_Key)(uint8_t edge, uint8_t keynum);

void setCallBackFunction(uint8_t i, CallBack_Key func);

#endif //TESTCLION_4X4MATRIXKEYBOARD_H
