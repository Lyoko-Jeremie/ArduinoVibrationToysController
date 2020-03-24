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


RhythmPlayer::RhythmPlayer(uint16_t rhythmIndex, uint16_t playMode) {
    this->init(rhythmIndex, playMode);
}

void RhythmPlayer::init(uint16_t rhythmIndex, uint16_t playMode) {
//    Serial.print("RhythmPlayer::RhythmPlayer");
//    Serial.print(rhythmIndex);
//    Serial.print("\t");
//    Serial.print(playMode);
//    Serial.print("\n");
    this->playMode = playMode;
    this->loadRhythm(rhythmIndex);
}

void RhythmPlayer::loadRhythm(uint16_t rhythmIndex) {
    if (RhythmTableSize <= rhythmIndex) {
        rhythmIndex = 0;
    }
//    Serial.print("jump to rhythm:");
//    Serial.print(this->rhythmIndex);
//    Serial.print("\n");
    this->rhythmIndex = rhythmIndex;
    this->rhythmP = getRhythmFromTable(rhythmIndex);
    this->rhythmTotalLength = getTotalLengthFromRhythm(this->rhythmP);
    this->rhythmMode = getModeFromRhythm(this->rhythmP);
    this->rhythmNoteIndex = 0;
    this->loadNextSummary(this->rhythmNoteIndex);
}

void RhythmPlayer::loadNextSummary(uint16_t nextSummaryIndex) {
    if (nextSummaryIndex >= this->rhythmTotalLength) {
        switch (this->playMode) {
            case PlayMode::PlayMode_Loop:
                nextSummaryIndex = 0;
                break;
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
                        break;
                    case RhythmMode::RhythmMode_Loop:
                    default:
                        nextSummaryIndex = 0;
                        break;
                }
                break;
        }
    }
    this->rhythmNoteIndex = nextSummaryIndex;
    this->rhythmNoteOffset = getRhythmOffsetFromRhythm(this->rhythmP, nextSummaryIndex);
    this->rhythmNoteIntervalCount = getIntervalCountFromRhythm(this->rhythmP, nextSummaryIndex);
    this->playIntervalCount = 0;
//    Serial.print("jump to summary:");
//    Serial.print(this->rhythmNoteIndex);
//    Serial.print("\n");
}

RhythmDataType RhythmPlayer::getNextNote() {
    if (this->playIntervalCount >= this->rhythmNoteIntervalCount) {
        // this summary play end, jump to next
        //Serial.print("this summary play end, jump to next\n");
        this->loadNextSummary(this->rhythmNoteIndex + 1);
    } else {
        ++this->playIntervalCount;
    }
    return this->rhythmNoteOffset;
}

