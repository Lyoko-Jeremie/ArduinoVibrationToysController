//
// Created by jeremie on 2020/03/23.
//

#include <Arduino.h>

#include "VibrationRhythmController.h"
#include "RhythmTable_intenal.h"


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
//    Serial.print("RhythmPlayer::RhythmPlayer");
//    Serial.print(rhythmIndex);
//    Serial.print("\t");
//    Serial.print(playMode);
//    Serial.print("\n");
    this->playMode = playMode;
    this->playDirection = playDirection;
    this->loadRhythm(rhythmIndex);
}

void RhythmPlayer::loadRhythm(uint16_t rhythmIndex) {
    if (RhythmTableSize <= rhythmIndex) {
        rhythmIndex = 0;
    }
    Serial.print("jump to rhythm:");
    Serial.print(rhythmIndex);
    Serial.print("\n");
    this->rhythmIndex = rhythmIndex;
    this->rhythmP = getRhythmFromTable(rhythmIndex);
    this->rhythmTotalLength = getTotalLengthFromRhythm(this->rhythmP);
    this->rhythmMode = getModeFromRhythm(this->rhythmP);
    this->rhythmDirection = getDirectionFromRhythm(this->rhythmP);
    this->playIntervalCount = 0;
    this->playSummaryIndex = this->calcNextSummaryIndex(true);
    Serial.print(F("jump to rhythm loadNextSummary\n"));
    this->loadNextSummary(this->playSummaryIndex);
}

void RhythmPlayer::loadNextRhythm() {
    Serial.print("loadNextRhythm\n");
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
                            Serial.print(
                                    F("loadNextRhythm RhythmMode_Reciprocate PlayDirection_Forward -> PlayDirection_Reverse\n"));
                            this->loadNextSummary(this->calcNextSummaryIndex(true));
                            return;
                        case PlayDirection::PlayDirection_Reverse:
                            this->playDirection = PlayDirection::PlayDirection_Forward;
                            this->playIntervalCount = 0;
                            Serial.print(
                                    F("loadNextRhythm RhythmMode_Reciprocate PlayDirection_Reverse -> PlayDirection_Forward\n"));
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
                        Serial.print(F("loadNextRhythm (this->playSummaryIndex >= this->rhythmTotalLength)\n"));
                        this->playSummaryIndex = 0;
                        this->playIntervalCount = 0;
                    } else {
                        Serial.print(F("loadNextRhythm !(this->playSummaryIndex >= this->rhythmTotalLength)\n"));
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
    Serial.print("jump to summary:");
    Serial.print(this->playSummaryIndex);
    Serial.print("\n");
}

int16_t RhythmPlayer::calcNextSummaryIndex(boolean initMode) {
    switch (this->playDirection) {
        case PlayDirection::PlayDirection_Forward:
            if (initMode) {
                Serial.print(F("calcNextSummaryIndex PlayDirection_Forward 0 \n"));
                return 0;
            }
            if (this->playSummaryIndex < 0) {
                Serial.print(F("calcNextSummaryIndex PlayDirection_Forward 1 \n"));
                return 0;
            }
            if (this->playSummaryIndex + 1 >= this->rhythmTotalLength) {
                Serial.print(F("calcNextSummaryIndex PlayDirection_Forward 2 \n"));
                this->loadNextRhythm();
                return this->calcNextSummaryIndex(true);
            }
            if (PlayDirection::PlayDirection_Forward == this->playDirection) {
                Serial.print(F("calcNextSummaryIndex PlayDirection_Forward 3 \n"));
                return this->playSummaryIndex + 1;
            }
            Serial.print(F("calcNextSummaryIndex PlayDirection_Forward 4 \n"));
            return this->playSummaryIndex + 1;
        case PlayDirection::PlayDirection_Reverse:
            if (initMode) {
                Serial.print(F("calcNextSummaryIndex PlayDirection_Reverse 1 \n"));
                return this->rhythmTotalLength - 1;
            }
            if (this->playSummaryIndex - 1 < 0) {
                Serial.print(F("calcNextSummaryIndex PlayDirection_Reverse 2 \n"));
                this->loadNextRhythm();
                return this->calcNextSummaryIndex(true);
            }
            if (this->playSummaryIndex >= this->rhythmTotalLength) {
                Serial.print(F("calcNextSummaryIndex PlayDirection_Reverse 3 \n"));
                return this->rhythmTotalLength - 1;
            }
            if (PlayDirection::PlayDirection_Reverse == this->playDirection) {
                Serial.print(F("calcNextSummaryIndex PlayDirection_Reverse 4 \n"));
                return this->playSummaryIndex - 1;
            }
            Serial.print(F("calcNextSummaryIndex PlayDirection_Reverse 5 \n"));
            return this->rhythmTotalLength - 1;
        case PlayDirection::PlayDirection_Random:
            Serial.print(F("calcNextSummaryIndex PlayDirection_Random 0 \n"));
            return random(0, this->rhythmTotalLength);
        case PlayDirection::PlayDirection_Default:
        default:
            Serial.print(F("calcNextSummaryIndex PlayDirection_Default 000 \n"));
            switch (this->rhythmDirection) {
                case RhythmDirection::RhythmDirection_Reverse:
                    if (initMode) {
                        Serial.print(F("calcNextSummaryIndex RhythmDirection_Reverse 0 \n"));
                        return this->rhythmTotalLength - 1;
                    }
                    if (this->playSummaryIndex - 1 < 0) {
                        Serial.print(F("calcNextSummaryIndex RhythmDirection_Reverse 1 \n"));
                        this->loadNextRhythm();
                        return this->calcNextSummaryIndex(true);
                    }
                    if (this->playSummaryIndex >= this->rhythmTotalLength) {
                        Serial.print(F("calcNextSummaryIndex RhythmDirection_Reverse 2 \n"));
                        return this->rhythmTotalLength - 1;
                    }
                    if (PlayDirection::PlayDirection_Reverse == this->playDirection) {
                        Serial.print(F("calcNextSummaryIndex RhythmDirection_Reverse 3 \n"));
                        return this->playSummaryIndex - 1;
                    }
                    Serial.print(F("calcNextSummaryIndex RhythmDirection_Reverse 4 \n"));
                    return this->playSummaryIndex - 1;
                case RhythmDirection::RhythmDirection_Random:
                    Serial.print(F("calcNextSummaryIndex RhythmDirection_Random 0 \n"));
                    return random(0, this->rhythmTotalLength);
                case RhythmDirection::RhythmDirection_Forward:
                default:
                    if (initMode) {
                        Serial.print(F("calcNextSummaryIndex RhythmDirection_Forward 0 \n"));
                        return 0;
                    }
                    if (this->playSummaryIndex < 0) {
                        Serial.print(F("calcNextSummaryIndex RhythmDirection_Forward 1 \n"));
                        return 0;
                    }
                    if (this->playSummaryIndex + 1 >= this->rhythmTotalLength) {
                        Serial.print(F("calcNextSummaryIndex RhythmDirection_Forward 2 \n"));
                        this->loadNextRhythm();
                        return this->calcNextSummaryIndex(true);
                    }
                    if (PlayDirection::PlayDirection_Forward == this->playDirection) {
                        Serial.print(F("calcNextSummaryIndex RhythmDirection_Forward 3 \n"));
                        return this->playSummaryIndex + 1;
                    }
                    Serial.print(F("calcNextSummaryIndex RhythmDirection_Forward 4 \n"));
                    return this->playSummaryIndex + 1;
                    break;
            }
            break;
    }
}

RhythmDataType RhythmPlayer::getNextNote() {
    if (this->playIntervalCount >= this->rhythmNoteIntervalCount) {
        // this summary play end, jump to next
        Serial.print(F("this summary play end, jump to next\n"));
//        this->loadNextSummary(this->playSummaryIndex + 1);
        this->loadNextSummary(this->calcNextSummaryIndex());
    } else {
        ++this->playIntervalCount;
    }
    return this->rhythmNoteOffset;
}


