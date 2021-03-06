//
// Created by jeremie on 2020/03/23.
//

#include <Arduino.h>

#include "VibrationRhythmController.h"
#include "RhythmTable_intenal.h"

#ifdef VibrationRhythmController_DEBUG_OUTPUT
#define debugP(any) Serial.print(F( any ))
#define debugPV(any) Serial.print(any)
#else
#define debugP(any) (void)0
#define debugPV(any) (void)0
#endif  // VibrationRhythmController_DEBUG_OUTPUT

void testCheckRhythmTable() {
    Serial.print("testCheckRhythmTable\n");
    for (int i = 0; i < RhythmTableSize; ++i) {
        Serial.print("========================\n");
        Serial.print("Rhythm (");
        Serial.print(i);
        Serial.print(") : ");
//        RhythmDataType *p = (RhythmDataType *) pgm_read_ptr(RhythmTable + i);
        RhythmDataType *p = getRhythmFromTable(i);
//        int16_t len = pgm_read_word(p + total_length_offset);
        int16_t len = getTotalLengthFromRhythm(p);
//        Serial.print("\t p:");
//        Serial.print((uint16_t)p);
        Serial.print("\t len:");
        Serial.print(len);
        Serial.print("\n{");
        for (int j = 0; j < len; ++j) {
//            RhythmDataType offset = pgm_read_word(p + rhythm_begin_offset + (j * 2) * rhythm_type_size);
            RhythmDataType offset = getRhythmOffsetFromRhythm(p, j);
//            RhythmDataType interval_count = pgm_read_word(p + rhythm_begin_offset + (j * 2 + 1) * rhythm_type_size);
            RhythmDataType interval_count = getIntervalCountFromRhythm(p, j);
            Serial.print("\n");
            Serial.print(offset);
            Serial.print(", ");
            Serial.print(interval_count);
            Serial.print(",");
        }
        Serial.print("}\n");
        Serial.print("========================\n");
    }
}


RhythmPlayer::RhythmPlayer(uint16_t rhythmIndex, uint16_t playMode, uint16_t playDirection) {
    this->init(rhythmIndex, playMode, playDirection);
}

void RhythmPlayer::init(uint16_t rhythmIndex, uint16_t playMode, uint16_t playDirection) {
    debugP("RhythmPlayer::RhythmPlayer");
    debugPV(rhythmIndex);
    debugP("\t");
    debugPV(playMode);
    debugP("\n");
    this->playMode = playMode;
    this->playDirection = playDirection;
    this->loadRhythm(rhythmIndex);
}

void RhythmPlayer::loadRhythm(uint16_t rhythmIndex) {
    if (RhythmTableSize <= rhythmIndex) {
        rhythmIndex = 0;
    }
//    Serial.print("jump to rhythm:");
//    Serial.print(rhythmIndex);
//    Serial.print("\n");
    debugP("jump to rhythm:");
    debugPV(rhythmIndex);
    debugP("\n");
    this->rhythmIndex = rhythmIndex;
    this->rhythmP = getRhythmFromTable(rhythmIndex);
    this->rhythmTotalLength = getTotalLengthFromRhythm(this->rhythmP);
    this->rhythmMode = getModeFromRhythm(this->rhythmP);
    this->rhythmDirection = getDirectionFromRhythm(this->rhythmP);
    this->playIntervalCount = 0;
    this->playSummaryIndex = this->calcNextSummaryIndex(true);
    debugP("jump to rhythm loadNextSummary\n");
    this->loadNextSummary(this->playSummaryIndex);
}

void RhythmPlayer::loadNextRhythm() {
    debugP("loadNextRhythm\n");
    switch (this->playMode) {
        case PlayMode::PlayMode_Loop:
            this->playIntervalCount = 0;
            return;
        case PlayMode::PlayMode_Next:
            this->loadRhythm(this->rhythmIndex + 1);
            return;
        case PlayMode::PlayMode_Random:
            this->loadRhythm((uint16_t) random(0, RhythmTableSize));
            return;
        case PlayMode::PlayMode_Default:
        default:
            switch (this->rhythmMode) {
                case RhythmMode::RhythmMode_Reciprocate:
                    switch (this->playDirection) {
                        case PlayDirection::PlayDirection_Default:
                        case PlayDirection::PlayDirection_Forward:
                            this->playDirection = PlayDirection::PlayDirection_Reverse;
                            this->playIntervalCount = 0;
                            debugP("loadNextRhythm RhythmMode_Reciprocate PlayDirection_Forward -> PlayDirection_Reverse\n");
                            this->loadNextSummary(this->calcNextSummaryIndex(true));
                            return;
                        case PlayDirection::PlayDirection_Reverse:
                            this->playDirection = PlayDirection::PlayDirection_Forward;
                            this->playIntervalCount = 0;
                            debugP("loadNextRhythm RhythmMode_Reciprocate PlayDirection_Reverse -> PlayDirection_Forward\n");
                            this->loadNextSummary(this->calcNextSummaryIndex(true));
                            return;
                        case PlayDirection::PlayDirection_Random:
                            this->loadRhythm((uint16_t) random(0, RhythmTableSize));
                            return;
                    }
                    return;
                case RhythmMode::RhythmMode_Loop:
                default:
                    if (this->playSummaryIndex >= this->rhythmTotalLength) {
                        debugP("loadNextRhythm (this->playSummaryIndex >= this->rhythmTotalLength)\n");
                        this->playSummaryIndex = 0;
                        this->playIntervalCount = 0;
                    } else {
                        debugP("loadNextRhythm !(this->playSummaryIndex >= this->rhythmTotalLength)\n");
                        this->playIntervalCount = 0;
                        this->loadRhythm(this->rhythmIndex + 1);
                    }
                    return;
                    break;
            }
            break;
    }
}

void RhythmPlayer::loadNextSummary(int16_t nextSummaryIndex) {
    if (nextSummaryIndex >= this->rhythmTotalLength || nextSummaryIndex < 0) {
        this->loadNextRhythm();
    }
    this->playSummaryIndex = nextSummaryIndex;
    this->rhythmNoteOffset = getRhythmOffsetFromRhythm(this->rhythmP, nextSummaryIndex);
    this->rhythmNoteIntervalCount = getIntervalCountFromRhythm(this->rhythmP, nextSummaryIndex);
    this->playIntervalCount = 0;
    debugP("jump to summary:");
    debugPV(this->playSummaryIndex);
    debugP("\n");
}

int16_t RhythmPlayer::calcNextSummaryIndex(boolean initMode) {
    switch (this->playDirection) {
        case PlayDirection::PlayDirection_Forward:
            if (initMode) {
                debugP("calcNextSummaryIndex PlayDirection_Forward 0 \n");
                return 0;
            }
            if (this->playSummaryIndex < 0) {
                debugP("calcNextSummaryIndex PlayDirection_Forward 1 \n");
                return 0;
            }
            if (this->playSummaryIndex + 1 >= this->rhythmTotalLength) {
                debugP("calcNextSummaryIndex PlayDirection_Forward 2 \n");
                this->loadNextRhythm();
                return this->calcNextSummaryIndex(true);
            }
            if (PlayDirection::PlayDirection_Forward == this->playDirection) {
                debugP("calcNextSummaryIndex PlayDirection_Forward 3 \n");
                return this->playSummaryIndex + 1;
            }
            debugP("calcNextSummaryIndex PlayDirection_Forward 4 \n");
            return this->playSummaryIndex + 1;
        case PlayDirection::PlayDirection_Reverse:
            if (initMode) {
                debugP("calcNextSummaryIndex PlayDirection_Reverse 1 \n");
                return this->rhythmTotalLength - 1;
            }
            if (this->playSummaryIndex - 1 < 0) {
                debugP("calcNextSummaryIndex PlayDirection_Reverse 2 \n");
                this->loadNextRhythm();
                return this->calcNextSummaryIndex(true);
            }
            if (this->playSummaryIndex >= this->rhythmTotalLength) {
                debugP("calcNextSummaryIndex PlayDirection_Reverse 3 \n");
                return this->rhythmTotalLength - 1;
            }
            if (PlayDirection::PlayDirection_Reverse == this->playDirection) {
                debugP("calcNextSummaryIndex PlayDirection_Reverse 4 \n");
                return this->playSummaryIndex - 1;
            }
            debugP("calcNextSummaryIndex PlayDirection_Reverse 5 \n");
            return this->rhythmTotalLength - 1;
        case PlayDirection::PlayDirection_Random:
            debugP("calcNextSummaryIndex PlayDirection_Random 0 \n");
            return random(0, this->rhythmTotalLength);
        case PlayDirection::PlayDirection_Default:
        default:
            debugP("calcNextSummaryIndex PlayDirection_Default 000 \n");
            switch (this->rhythmDirection) {
                case RhythmDirection::RhythmDirection_Reverse:
                    if (initMode) {
                        debugP("calcNextSummaryIndex RhythmDirection_Reverse 0 \n");
                        return this->rhythmTotalLength - 1;
                    }
                    if (this->playSummaryIndex - 1 < 0) {
                        debugP("calcNextSummaryIndex RhythmDirection_Reverse 1 \n");
                        this->loadNextRhythm();
                        return this->calcNextSummaryIndex(true);
                    }
                    if (this->playSummaryIndex >= this->rhythmTotalLength) {
                        debugP("calcNextSummaryIndex RhythmDirection_Reverse 2 \n");
                        return this->rhythmTotalLength - 1;
                    }
                    if (PlayDirection::PlayDirection_Reverse == this->playDirection) {
                        debugP("calcNextSummaryIndex RhythmDirection_Reverse 3 \n");
                        return this->playSummaryIndex - 1;
                    }
                    debugP("calcNextSummaryIndex RhythmDirection_Reverse 4 \n");
                    return this->playSummaryIndex - 1;
                case RhythmDirection::RhythmDirection_Random:
                    debugP("calcNextSummaryIndex RhythmDirection_Random 0 \n");
                    return random(0, this->rhythmTotalLength);
                case RhythmDirection::RhythmDirection_Forward:
                default:
                    if (initMode) {
                        debugP("calcNextSummaryIndex RhythmDirection_Forward 0 \n");
                        return 0;
                    }
                    if (this->playSummaryIndex < 0) {
                        debugP("calcNextSummaryIndex RhythmDirection_Forward 1 \n");
                        return 0;
                    }
                    if (this->playSummaryIndex + 1 >= this->rhythmTotalLength) {
                        debugP("calcNextSummaryIndex RhythmDirection_Forward 2 \n");
                        this->loadNextRhythm();
                        return this->calcNextSummaryIndex(true);
                    }
                    if (PlayDirection::PlayDirection_Forward == this->playDirection) {
                        debugP("calcNextSummaryIndex RhythmDirection_Forward 3 \n");
                        return this->playSummaryIndex + 1;
                    }
                    debugP("calcNextSummaryIndex RhythmDirection_Forward 4 \n");
                    return this->playSummaryIndex + 1;
                    break;
            }
            break;
    }
}

RhythmDataType RhythmPlayer::getNextNote() {
    if (this->playIntervalCount >= this->rhythmNoteIntervalCount) {
        // this summary play end, jump to next
        debugP("this summary play end, jump to next\n");
//        this->loadNextSummary(this->playSummaryIndex + 1);
        this->loadNextSummary(this->calcNextSummaryIndex());
    } else {
        ++this->playIntervalCount;
    }
    return this->rhythmNoteOffset;
}

uint16_t RhythmPlayer::getNowRhythmIndex() {
    return this->rhythmIndex;
}

void RhythmPlayer::forceNextRhythm() {
    uint16_t newI = this->rhythmIndex + 1;
    if (newI >= RhythmTableSize) {
        newI = 0;
    }
    this->init(newI, this->playMode, this->playDirection);
}

void RhythmPlayer::forceLastRhythm() {
    uint16_t newI = this->rhythmIndex - 1;
    if (0 == this->rhythmIndex) {
        newI = this->rhythmIndex - 1;
    }
    this->init(newI, this->playMode, this->playDirection);
}

void RhythmPlayer::setModeOnNowRhythm(uint16_t playMode) {
    this->playMode = playMode;
}

void RhythmPlayer::setDirectionOnNowRhythm(uint16_t playDirection) {
    this->playDirection = playDirection;
}


