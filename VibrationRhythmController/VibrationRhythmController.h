//
// Created by jeremie on 2020/03/23.
//

#ifndef H_VIBRATIONRHYTHMCONTROLLER_H
#define H_VIBRATIONRHYTHMCONTROLLER_H

#include "RhythmTable.h"

void testCheckRhythmTable();

enum PlayMode {
    PlayMode_Default = 0,
    PlayMode_Loop = 1,
    PlayMode_Next = 2,
    PlayMode_Random = 3,
};

class RhythmPlayer {
    // rhythm info
    uint16_t rhythmIndex;
    RhythmDataType *rhythmP;
    // rhythm head
    uint16_t rhythmTotalLength;
    uint16_t rhythmMode;
    // rhythm summary info
    uint16_t rhythmNoteIndex;
    uint16_t rhythmNoteOffset{0};
    uint16_t rhythmNoteIntervalCount{1};
    // playing info
    uint16_t playMode{PlayMode::PlayMode_Default};
    uint16_t playIntervalCount{0};
public:
    void debugPrint() {
        Serial.print("RhythmPlayer :");
        Serial.print(this->rhythmIndex);
        Serial.print("\t");
//        Serial.print((uint16_t) this->rhythmP);
//        Serial.print("\t");
        Serial.print(this->rhythmTotalLength);
        Serial.print("\t");
        Serial.print(this->rhythmMode);
        Serial.print("\n");

        Serial.print(this->rhythmNoteIndex);
        Serial.print("\t");
        Serial.print(this->rhythmNoteOffset);
        Serial.print("\t");
        Serial.print(this->rhythmNoteIntervalCount);
        Serial.print("\t");
        Serial.print(this->playMode);
        Serial.print("\t");
        Serial.print(this->playIntervalCount);
//        Serial.print("\t");
        Serial.print("\n");
    }

public:
    RhythmPlayer(uint16_t rhythmIndex, uint16_t playMode = PlayMode::PlayMode_Default);

    void init(uint16_t rhythmIndex, uint16_t playMode);

    void loadRhythm(uint16_t rhythmIndex);

protected:
    void loadNextSummary(uint16_t nextSummaryIndex);

public:
    RhythmDataType getNextNote();

};


#endif //H_VIBRATIONRHYTHMCONTROLLER_H
