#include <Arduino.h>

#include "4x4MatrixKeyboard/4x4MatrixKeyboard.h"
#include "VibrationRhythmController/VibrationRhythmController.h"
#include "BoolBitStorage/BoolBitStorage.h"

// test PWM

#include <Servo.h>

#define MaxServosNum 3

Servo servos[MaxServosNum];
RhythmPlayer *rp;

void key_test(uint8_t edge, uint8_t keynum);

void regKeyCallBack();

void setup() {

//    Serial.begin(9600);
//    BoolBit8 b = 0;
//
//    delay(1000);
//    for (int i = 0; i < 8; ++i) {
//        Serial.print("---- i: \t");
//        Serial.print(i);
//        Serial.print('\t');
//        Serial.print(getBoolBit(b, i));
//        Serial.print('\t');
//        setBoolBit(b, i, 1);
//        Serial.print(getBoolBit(b, i));
//        Serial.print('\t');
//        Serial.print(b);
//        Serial.print('\t');
//        setBoolBit(b, i, 0);
//        Serial.print(getBoolBit(b, i));
//        Serial.print('\t');
//        Serial.print(b);
//        Serial.print('\t');
//        setBoolBit(b, i, 1);
//        Serial.print(getBoolBit(b, i));
//        Serial.print('\t');
//        Serial.print(b);
//        Serial.print('\n');
//        setBoolBit(b, i, 0);
//    }
//
//    return;

    // load a random seed from D13
    randomSeed(analogRead(13));

    // init RhythmPlayer
    rp = new RhythmPlayer[MaxServosNum]{
            {0, PlayMode::PlayMode_Default, PlayDirection::PlayDirection_Default},
            {0, PlayMode::PlayMode_Default, PlayDirection::PlayDirection_Default},
            {0, PlayMode::PlayMode_Default, PlayDirection::PlayDirection_Default},
    };

    // init Keyboard
    initPortState();
    regKeyCallBack();

    // init servos
    servos[0].attach(10);
    servos[1].attach(11);
    servos[2].attach(12);

    // init serial
    Serial.begin(9600);

}


#define isTotalStart 0
#define isTotalPause 1
#define isServosPauseBase 2
#define isServos1Pause (isServosPauseBase+0)
#define isServos2Pause (isServosPauseBase+1)
#define isServos3Pause (isServosPauseBase+2)
BoolBit8 state = 0;

#define maxLevel 2000
#define minLevel 1000
#define baseLevel 1500
int16_t servosLevel[MaxServosNum] = {
        baseLevel,
        baseLevel,
        baseLevel,
};
#define maxLevelOffset 10
#define mixLevelOffset 0.1
#define baseLevelOffset 1
#define levelOffsetAddStep 1.2
#define levelOffsetSubStep 0.833
float levelOffset[MaxServosNum] = {
        baseLevelOffset,
        baseLevelOffset,
        baseLevelOffset,
};

void loop() {

    ScanKeyAndCallKeyCallBackFunction();

    if (getBoolBit(state, isTotalStart) & !getBoolBit(state, isTotalPause)) {
        for (int i = 0; i < MaxServosNum; ++i) {
            if (!getBoolBit(state, isServosPauseBase + i)) {
                // servos[i].writeMicroseconds(rp[i].getNextNote());
                int16_t orgL = rp[i].getNextNote();
                servosLevel[i] = (int16_t) ((float) orgL * levelOffset[i]) + baseLevel;
                if (servosLevel[i] > maxLevel) {
                    servosLevel[i] = maxLevel;
                }
                if (servosLevel[i] < minLevel) {
                    servosLevel[i] = minLevel;
                }
                servos[i].writeMicroseconds(servosLevel[i]);
            }
        }
    } else {
    }

    delay(10);
}

void key_test(uint8_t edge, uint8_t keynum) {
    Serial.print("key_test = ");
    Serial.print(keynum + 1); //
    Serial.print(" on ");
    Serial.print(edge);
    Serial.print("\n");
}

int8_t getServoNum() {
    for (uint8_t i = 0; i < MaxServosNum; ++i) {
        if (GetKeyState(i)) {
            return i;
        }
    }
    return -1;
}

//
void key_total_start(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    if (!getBoolBit(state, isTotalStart)) {
        // init Rhythm
        for (int i = 0; i < MaxServosNum; ++i) {
            rp[i].init(0, PlayMode::PlayMode_Default, PlayDirection::PlayDirection_Default);
        }
    }
    setBoolBit(state, isTotalStart, 1);
    if (getBoolBit(state, isTotalPause)) {
        // last in pause state
        setBoolBit(state, isTotalPause, 0);
    }
    for (int j = 0; j < MaxServosNum; ++j) {
        levelOffset[j] = baseLevelOffset;
    }
}

void key_total_stop(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    setBoolBit(state, isTotalStart, 0);
    setBoolBit(state, isTotalPause, 0);
    for (int j = 0; j < MaxServosNum; ++j) {
        levelOffset[j] = baseLevelOffset;
    }
    // clean Rhythm
    for (int i = 0; i < MaxServosNum; ++i) {
        rp[i].init(0, PlayMode::PlayMode_Default, PlayDirection::PlayDirection_Default);
    }
}

void key_total_pause(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    uint8_t lastState = getBoolBit(state, isTotalPause);
    setBoolBit(state, isTotalPause, !lastState);
}

void key_total_strength(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    for (int i = 0; i < MaxServosNum; ++i) {
        levelOffset[i] *= levelOffsetAddStep;
        if (levelOffset[i] > maxLevelOffset) {
            levelOffset[i] = maxLevelOffset;
        }
    }
}

void key_total_weak(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    for (int i = 0; i < MaxServosNum; ++i) {
        levelOffset[i] *= levelOffsetSubStep;
        if (levelOffset[i] < mixLevelOffset) {
            levelOffset[i] = mixLevelOffset;
        }
    }
}

void key_one_next(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    for (uint8_t i = 0; i < MaxServosNum; ++i) {
        if (GetKeyState(i)) {
            rp[i].forceNextRhythm();
        }
    }
}

void key_one_last(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    for (uint8_t i = 0; i < MaxServosNum; ++i) {
        if (GetKeyState(i)) {
            rp[i].forceLastRhythm();
        }
    }
}

//
void key_one_pause(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    for (uint8_t i = 0; i < MaxServosNum; ++i) {
        if (GetKeyState(i)) {
            uint8_t lastState = getBoolBit(state, isServosPauseBase + i);
            setBoolBit(state, isServosPauseBase + i, !lastState);
        }
    }
}

void key_one_strength(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    for (int i = 0; i < MaxServosNum; ++i) {
        if (GetKeyState(i)) {
            levelOffset[i] *= levelOffsetAddStep;
            if (levelOffset[i] > maxLevelOffset) {
                levelOffset[i] = maxLevelOffset;
            }
        }
    }
}

void key_one_weak(uint8_t edge, uint8_t keynum) {
    if (!edge) {
        return;
    }
    for (int i = 0; i < MaxServosNum; ++i) {
        if (GetKeyState(i)) {
            levelOffset[i] *= levelOffsetSubStep;
            if (levelOffset[i] < mixLevelOffset) {
                levelOffset[i] = mixLevelOffset;
            }
        }
    }
}

void key_reset_levelOffset(uint8_t edge, uint8_t keynum) {
    uint8_t n = getServoNum();
    if (n == -1) {
        // if any Servo be selected
        // reset all
        for (int i = 0; i < MaxServosNum; ++i) {
            levelOffset[i] = baseLevelOffset;
        }
    } else {
        for (int i = 0; i < MaxServosNum; ++i) {
            if (GetKeyState(i)) {
                levelOffset[i] = baseLevelOffset;
            }
        }
    }
}

//
void key_total_next(uint8_t edge, uint8_t keynum) {}

void key_total_last(uint8_t edge, uint8_t keynum) {}

/**************************************
 *    |0       |1      |2      |3
 * ---|--------|-------|-------|-------
 * 0  |1       |2      |3      |o_pause
 * 4  |o_next  |o_last |o_st   |o_wk
 * 8  |t_str   |t_weak |t_next |t_last
 * 12 |t_start |t_stop |t_pause|reset_levelOffset
 * ************************************
 *
 */
void regKeyCallBack() {
    setCallBackFunction(3, key_one_pause);
    setCallBackFunction(4, key_one_next);
    setCallBackFunction(5, key_one_last);
    setCallBackFunction(6, key_one_strength);
    setCallBackFunction(7, key_one_weak);
    setCallBackFunction(8, key_total_strength);
    setCallBackFunction(9, key_total_weak);
    setCallBackFunction(10, key_total_next);
    setCallBackFunction(11, key_total_last);
    setCallBackFunction(12, key_total_start);
    setCallBackFunction(13, key_total_stop);
    setCallBackFunction(14, key_total_pause);
    setCallBackFunction(15, key_reset_levelOffset);
}
