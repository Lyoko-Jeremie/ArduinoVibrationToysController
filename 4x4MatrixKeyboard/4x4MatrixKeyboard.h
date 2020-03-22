//
// Created by jeremie on 2020/03/22.
//

#ifndef H_4X4MATRIXKEYBOARD_H
#define H_4X4MATRIXKEYBOARD_H

#include "MacroDefine.h"


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
 * the key callback function type
 * set this function to a key use ```setCallBackFunction()```
 * when a key change detected, will call the callback
 * @param edge      1 when key press, 0 when key release, only will be called at key change edge
 * @param keynum    the key i
 */
typedef void (CallBack_Key)(uint8_t edge, uint8_t keynum);

/**
 * set a function as the callback when a key state change detected
 *
 * if not care about a key, not set it will well.
 *
 * @param i     which key
 * @param func  the callback, must as CallBack_Key type
 */
void setCallBackFunction(uint8_t i, CallBack_Key func);

/**
 * max support ROWS
 */
#define D_4X4MATRIXKEYBOARD_MAX_ROWS 4
/**
 * max support COLS
 */
#define D_4X4MATRIXKEYBOARD_MAX_COLS 4

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

#endif //H_4X4MATRIXKEYBOARD_H
