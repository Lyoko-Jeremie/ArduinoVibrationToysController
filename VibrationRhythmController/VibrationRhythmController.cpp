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
//    Serial.print("jump to rhythm:");
//    Serial.print(rhythmIndex);
//    Serial.print("\n");
    this->rhythmIndex = rhythmIndex;
    this->rhythmP = getRhythmFromTable(rhythmIndex);
    this->rhythmTotalLength = getTotalLengthFromRhythm(this->rhythmP);
    this->rhythmMode = getModeFromRhythm(this->rhythmP);
    this->rhythmDirection = getDirectionFromRhythm(this->rhythmP);
    this->playIntervalCount = 0;
    this->playSummaryIndex = this->calcNextSummaryIndex(true);
    this->loadNextSummary(this->playSummaryIndex);
}

void RhythmPlayer::loadNextRhythm() {
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
                    // TODO
                    return;
                case RhythmMode::RhythmMode_Loop:
                default:
                    this->playIntervalCount = 0;
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
//    Serial.print("jump to summary:");
//    Serial.print(this->playSummaryIndex);
//    Serial.print("\n");
}

int16_t RhythmPlayer::calcNextSummaryIndex(boolean initMode) {
    switch (this->playDirection) {
        case PlayDirection::PlayDirection_Forward:
            if (initMode) {
                return 0;
            }
            if (this->playSummaryIndex < 0) {
                return 0;
            }
            if (this->playSummaryIndex + 1 >= this->rhythmTotalLength) {
                this->loadNextRhythm();
            }
            return this->playSummaryIndex + 1;
        case PlayDirection::PlayDirection_Reverse:
            if (initMode) {
                return this->rhythmTotalLength - 1;
            }
            if (this->playSummaryIndex - 1 < 0) {
                this->loadNextRhythm();
            }
            if (this->playSummaryIndex >= this->rhythmTotalLength) {
                return this->rhythmTotalLength - 1;
            }
            return this->playSummaryIndex - 1;
        case PlayDirection::PlayDirection_Random:
            return random(0, this->rhythmTotalLength);
        case PlayDirection::PlayDirection_Default:
        default:
            switch (this->rhythmDirection) {
                case RhythmDirection::RhythmDirection_Reverse:
                    if (initMode) {
                        return this->rhythmTotalLength - 1;
                    }
                    if (this->playSummaryIndex - 1 < 0) {
                        this->loadNextRhythm();
                    }
                    if (this->playSummaryIndex >= this->rhythmTotalLength) {
                        return this->rhythmTotalLength - 1;
                    }
                    return this->playSummaryIndex - 1;
                case RhythmDirection::RhythmDirection_Random:
                    return random(0, this->rhythmTotalLength);
                case RhythmDirection::RhythmDirection_Forward:
                default:
                    if (initMode) {
                        return 0;
                    }
                    if (this->playSummaryIndex < 0) {
                        return 0;
                    }
                    if (this->playSummaryIndex + 1 >= this->rhythmTotalLength) {
                        this->loadNextRhythm();
                    }
                    return this->playSummaryIndex + 1;
                    break;
            }
            break;
    }
}

RhythmDataType RhythmPlayer::getNextNote() {
    if (this->playIntervalCount >= this->rhythmNoteIntervalCount) {
        // this summary play end, jump to next
        //Serial.print("this summary play end, jump to next\n");
//        this->loadNextSummary(this->playSummaryIndex + 1);
        this->loadNextSummary(this->calcNextSummaryIndex());
    } else {
        ++this->playIntervalCount;
    }
    return this->rhythmNoteOffset;
}


