//
// Created by jeremie on 2020/03/22.
//

#include <Arduino.h>

#include "4x4MatrixKeyboard.h"

// follow code port from https://github.com/Lyoko-Jeremie/52/blob/master/main.c

uint8_t isInited = false;


#ifdef USE_SET_PIN
int row0 = 5;
int row1 = 4;
int row2 = 3;
int row3 = 2;

int col0 = 6;
int col1 = 7;
int col2 = 8;
int col3 = 9;

void SetPin(
        int _row0,
        int _row1,
        int _row2,
        int _row3,
        int _col0,
        int _col1,
        int _col2,
        int _col3
) {
    if (!isInited) {
        row0 = _row0;
        row1 = _row1;
        row2 = _row2;
        row3 = _row3;
        col0 = _col0;
        col1 = _col1;
        col2 = _col2;
        col3 = _col3;
    }
}
#else   // USE_SET_PIN
const int row0 = PREDEFINE_PIN_ROW_0;
const int row1 = PREDEFINE_PIN_ROW_1;
const int row2 = PREDEFINE_PIN_ROW_2;
const int row3 = PREDEFINE_PIN_ROW_3;

const int col0 = PREDEFINE_PIN_COL_0;
const int col1 = PREDEFINE_PIN_COL_1;
const int col2 = PREDEFINE_PIN_COL_2;
const int col3 = PREDEFINE_PIN_COL_3;

//const int row0 = 5;
//const int row1 = 4;
//const int row2 = 3;
//const int row3 = 2;
//
//const int col0 = 6;
//const int col1 = 7;
//const int col2 = 8;
//const int col3 = 9;
#endif  // USE_SET_PIN  USE_PREDEFINE_SET_PIN

#ifdef USE_MAX_VALID_ROW_COL
uint8_t MaxValidRow = D_4X4MATRIXKEYBOARD_MAX_ROWS;
uint8_t MaxValidCol = D_4X4MATRIXKEYBOARD_MAX_COLS;

void setMaxValidRowCol(uint8_t row, uint8_t col) {
    if (!isInited) {
        if (row > D_4X4MATRIXKEYBOARD_MAX_ROWS) {
            MaxValidRow = D_4X4MATRIXKEYBOARD_MAX_ROWS;
        } else if (row < 0) {
            MaxValidRow = 0;
        } else {
            MaxValidRow = row;
        }
        if (col > D_4X4MATRIXKEYBOARD_MAX_COLS) {
            MaxValidCol = D_4X4MATRIXKEYBOARD_MAX_COLS;
        } else if (col < 0) {
            MaxValidCol = 0;
        } else {
            MaxValidCol = col;
        }
    }
}
#else   // USE_MAX_VALID_ROW_COL
const uint8_t MaxValidRow = D_4X4MATRIXKEYBOARD_MAX_ROWS;
const uint8_t MaxValidCol = D_4X4MATRIXKEYBOARD_MAX_COLS;
#endif  // USE_MAX_VALID_ROW_COL

/**
 * must call before ScanKeyAndCallKeyCallBackFunction()
 */
void initPortState() {
    isInited = true;
    if (MaxValidRow > 0) pinMode(row0, OUTPUT);
    if (MaxValidRow > 1) pinMode(row1, OUTPUT);
    if (MaxValidRow > 2) pinMode(row2, OUTPUT);
    if (MaxValidRow > 3) pinMode(row3, OUTPUT);
    if (MaxValidCol > 0) pinMode(col0, INPUT_PULLUP);
    if (MaxValidCol > 1) pinMode(col1, INPUT_PULLUP);
    if (MaxValidCol > 2) pinMode(col2, INPUT_PULLUP);
    if (MaxValidCol > 3) pinMode(col3, INPUT_PULLUP);
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
    for (uint8_t i = 0; i < MaxValidRow; ++i) {
        if (MaxValidRow > 0) digitalWrite(row0, HIGH);
        if (MaxValidRow > 1) digitalWrite(row1, HIGH);
        if (MaxValidRow > 2) digitalWrite(row2, HIGH);
        if (MaxValidRow > 3) digitalWrite(row3, HIGH);
        if (0 == i) digitalWrite(row0, LOW);
        if (1 == i) digitalWrite(row1, LOW);
        if (2 == i) digitalWrite(row2, LOW);
        if (3 == i) digitalWrite(row3, LOW);
        for (uint8_t j = 0; j < MaxValidCol; ++j) {
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
            uint8_t stableState = (AKstate[i / 4][i % 4] & 0x2u) ? 1u : 0u;
            if (CBKeyList[i]) {
                (CBKeyList[i])(stableState, i);
            }
        }
    }
}


void ScanKeyAndCallKeyCallBackFunction() {
    ArrayKeyScan();
    CallKeyCallBackFunction();
}

uint8_t GetKeyState(uint8_t i) {
    return AKstate[i / 4][i % 4] & 0x2u;
}
