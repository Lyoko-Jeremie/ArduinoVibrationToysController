//
// Created by jeremie on 2020/03/22.
//

// follow code port from https://github.com/Lyoko-Jeremie/52/blob/master/main.c

#include <Arduino.h>

#include "4x4MatrixKeyboard.h"

#ifdef USE_SET_PIN

void MATRIXKEYBOARD4X4::SetPin(
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

#endif // USE_SET_PIN

#ifdef USE_MAX_VALID_ROW_COL
void MATRIXKEYBOARD4X4::setMaxValidRowCol(uint8_t row, uint8_t col) {
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
#endif // USE_MAX_VALID_ROW_COL

/**
 * must call before ScanKeyAndCallKeyCallBackFunction()
 */
void MATRIXKEYBOARD4X4::initPortState() {
#ifdef NEED_CHECK_isInited
    isInited = true;
#endif // NEED_CHECK_isInited
    if (MaxValidRow > 0) pinMode(row0, OUTPUT);
    if (MaxValidRow > 1) pinMode(row1, OUTPUT);
    if (MaxValidRow > 2) pinMode(row2, OUTPUT);
    if (MaxValidRow > 3) pinMode(row3, OUTPUT);
    if (MaxValidCol > 0) pinMode(col0, INPUT_PULLUP);
    if (MaxValidCol > 1) pinMode(col1, INPUT_PULLUP);
    if (MaxValidCol > 2) pinMode(col2, INPUT_PULLUP);
    if (MaxValidCol > 3) pinMode(col3, INPUT_PULLUP);
}

uint8_t MATRIXKEYBOARD4X4::GetColState(uint8_t n) {
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

void MATRIXKEYBOARD4X4::ArrayKeyScan() {
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


void MATRIXKEYBOARD4X4::setCallBackFunction(uint8_t i, CallBack_Key func) {
    CBKeyList[i] = func;
}

// 调用按键回调函数
void MATRIXKEYBOARD4X4::CallKeyCallBackFunction() {
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


void MATRIXKEYBOARD4X4::ScanKeyAndCallKeyCallBackFunction() {
    ArrayKeyScan();
    CallKeyCallBackFunction();
}

uint8_t MATRIXKEYBOARD4X4::GetKeyState(uint8_t i) {
    return (AKstate[i / 4][i % 4] & 0x2u) ? 1u : 0u;
}
