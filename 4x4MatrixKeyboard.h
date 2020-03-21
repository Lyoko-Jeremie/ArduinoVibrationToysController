//
// Created by jeremie on 2020/03/22.
//


#ifndef TESTCLION_4X4MATRIXKEYBOARD_H
#define TESTCLION_4X4MATRIXKEYBOARD_H

void initPortState();
void ArrayKeyScan();
void CallKeyCallBackFunction();

typedef void (CallBack_Key)(uint8_t edge, uint8_t keynum);

void setCallBackFunction(uint8_t i, CallBack_Key func);

#endif //TESTCLION_4X4MATRIXKEYBOARD_H
