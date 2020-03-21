//
// Created by jeremie on 2020/03/22.
//

#include <Arduino.h>

#include "4x4MatrixKeyboard.h"

// follow code port from https://github.com/Lyoko-Jeremie/52/blob/master/main.c

const int row0 = 5;
const int row1 = 4;
const int row2 = 3;
const int row3 = 2;

const int col0 = 6;
const int col1 = 7;
const int col2 = 8;
const int col3 = 9;

void initPortState() {
    pinMode(row0, OUTPUT);
    pinMode(row1, OUTPUT);
    pinMode(row2, OUTPUT);
    pinMode(row3, OUTPUT);
    pinMode(col0, INPUT_PULLUP);
    pinMode(col1, INPUT_PULLUP);
    pinMode(col2, INPUT_PULLUP);
    pinMode(col3, INPUT_PULLUP);
}

// 0x1 刚检测到一次疑似状态变更（防抖）
// 0x2 已确认的当前状态
// 0x4 刚发生了一次确认的状态变更
uint8_t AKstate[4][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
};

uint8_t GetColState(uint8_t n) {
    switch (n) {
        case 0:
            return digitalRead(col0);
        case 1:
            return digitalRead(col1);
        case 2:
            return digitalRead(col2);
        case 3:
            return digitalRead(col3);
        default:
            return 0;
    }
}

void ArrayKeyScan() {
    char i, j;
    for (i = 0; i != 4; ++i) {
        digitalWrite(row0, HIGH);
        digitalWrite(row1, HIGH);
        digitalWrite(row2, HIGH);
        digitalWrite(row3, HIGH);
        if (0 == i) digitalWrite(row0, LOW);
        if (1 == i) digitalWrite(row1, LOW);
        if (2 == i) digitalWrite(row2, LOW);
        if (3 == i) digitalWrite(row3, LOW);
        for (j = 0; j != 4; ++j) {
            if (!GetColState(j)) {
                // 检测到接通
                if (AKstate[i][j] & 0x2u) {
                    // 且本就为按下状态
                    AKstate[i][j] = 0x2u;  // 清除flag0x1 flag0x4 标记flag0x2
                } else {
                    // 且本不为按下状态
                    if (AKstate[i][j] & 0x1u) {
                        // 上次已经标记
                        AKstate[i][j] = 0x2u | 0x4u;  // 标记flag0x4 flag0x2 清除flag0x1
                    } else {
                        // 标记flag0x1
                        AKstate[i][j] = 0x1u;
                    }
                }
            } else {
                // 检测到未接通
                if (!(AKstate[i][j] & 0x2u)) {
                    if (AKstate[i][j] & 0x1u) {
                        // 上次已经标记
                        AKstate[i][j] = 0x4u;  // 清除flag0x1 flag0x2 标记flag0x4
                    } else {
                        // 且本就为未按下状态
                        AKstate[i][j] = 0x00u;  // 清除flag0x1 flag0x2 flag0x4
                    }
                } else {
                    // 且本为按下状态
                    // 标记flag0x1
                    AKstate[i][j] = 0x1u;   // 标记flag0x1
                }
            }
        }
    }
}


// 按键回调函数表
// 前16个为矩阵键盘 0~15
CallBack_Key *CBKeyList[16] = {nullptr};

void setCallBackFunction(uint8_t i, CallBack_Key func) {
    CBKeyList[i] = func;
}

// 调用按键回调函数
void CallKeyCallBackFunction() {
    uint8_t i;
    // 矩阵键盘
    for (i = 0; i != 16; ++i) {
        // 如果矩阵键盘此处是一个已确认的上升沿
        if (AKstate[i / 4][i % 4] & 0x4u) {
            uint8_t edge = (AKstate[i / 4][i % 4] & 0x2u) ? 1u : 0u;
            if (CBKeyList[i]) {
                (CBKeyList[i])(edge, i);
            }
        }
    }
}


void ScanKeyAndCallKeyCallBackFunction() {
    ArrayKeyScan();
    CallKeyCallBackFunction();
}
