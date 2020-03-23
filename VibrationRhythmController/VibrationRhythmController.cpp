//
// Created by jeremie on 2020/03/23.
//

#include <Arduino.h>

#include "VibrationRhythmController.h"
#include "RhythmTable.h"


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
        Serial.print("\t p:");
        Serial.print((uint16_t)p);
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



