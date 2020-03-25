//
// Created by jeremie on 2020/03/22.
//

#ifndef H_4X4MATRIXKEYBOARD_H
#define H_4X4MATRIXKEYBOARD_H

#include "MacroDefine.h"

/**
 * the key callback function type
 * set this function to a key use ```setCallBackFunction()```
 * when a key change detected, will call the callback
 * @param edge      1 when key press, 0 when key release, only will be called at key change edge
 * @param keynum    the key i
 */
typedef void (CallBack_Key)(uint8_t edge, uint8_t keynum);

/**
 * max support ROWS
 */
#define D_4X4MATRIXKEYBOARD_MAX_ROWS 4
/**
 * max support COLS
 */
#define D_4X4MATRIXKEYBOARD_MAX_COLS 4


class MATRIXKEYBOARD4X4 {
protected:
    // 按键回调函数表
    // 前16个为矩阵键盘 0~15
    CallBack_Key *CBKeyList[16] = {nullptr};

    // 0x1 刚检测到一次疑似状态变更（防抖）
    // 0x2 已确认的当前状态
    // 0x4 刚发生了一次确认的状态变更
    uint8_t AKstate[4][4] = {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
    };

#ifdef NEED_CHECK_isInited
    uint8_t isInited = false;
#endif // NEED_CHECK_isInited

#ifdef USE_SET_PIN
    int row0 = 5;
    int row1 = 4;
    int row2 = 3;
    int row3 = 2;

    int col0 = 6;
    int col1 = 7;
    int col2 = 8;
    int col3 = 9;
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
#else   // USE_MAX_VALID_ROW_COL
    const uint8_t MaxValidRow = D_4X4MATRIXKEYBOARD_MAX_ROWS;
    const uint8_t MaxValidCol = D_4X4MATRIXKEYBOARD_MAX_COLS;
#endif  // USE_MAX_VALID_ROW_COL


public:
#ifdef USE_SET_PIN
    /**
     * must call before initPortState()
     * @param _row0
     * @param _row1
     * @param _row2
     * @param _row3
     * @param _col0
     * @param _col1
     * @param _col2
     * @param _col3
     */
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
#else   // USE_SET_PIN
    /*nothing*/
#endif  // USE_SET_PIN

    void initPortState();

    void ScanKeyAndCallKeyCallBackFunction();

    /**
     * get the key state
     * can call in any time
     * @param i
     * @return the key state
     */
    uint8_t GetKeyState(uint8_t i);

    /**
     * set a function as the callback when a key state change detected
     *
     * if not care about a key, not set it will well.
     *
     * @param i     which key
     * @param func  the callback, must as CallBack_Key type
     */
    void setCallBackFunction(uint8_t i, CallBack_Key func);

#ifdef USE_MAX_VALID_ROW_COL
    /**
     * must call before initPortState()
     *
     * if not call it, use the ```D_4X4MATRIXKEYBOARD_MAX_ROWS``` and ```D_4X4MATRIXKEYBOARD_MAX_COLS``` use as default
     * @param row   the valid row, must small or equal D_4X4MATRIXKEYBOARD_MAX_ROWS
     * @param col   the valid col, must small or equal D_4X4MATRIXKEYBOARD_MAX_COLS
     */
    void setMaxValidRowCol(uint8_t row, uint8_t col);
#else   // USE_MAX_VALID_ROW_COL
/*nothing*/
#endif  // USE_MAX_VALID_ROW_COL


protected:
    uint8_t GetColState(uint8_t n);

    void ArrayKeyScan();

    void CallKeyCallBackFunction();

};

#endif //H_4X4MATRIXKEYBOARD_H
