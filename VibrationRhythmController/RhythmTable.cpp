//
// Created by jeremie on 2020/03/23.
//

#include <Arduino.h>

#include "RhythmTable.h"

// the data example
//const PROGMEM int16_t Rhythm[2 + 8 * 2] = {
//        // total length, mode,      <==== the Rhythm info head
//        8, 0,
//        // reserve, reserve,      <==== mode info
//        0, 0,
//        // rhythm offset, interval count,      <==== 2 number made a note group
//        +0, 5,
//        +100, 5,
//        +500, 5,
//        +1000, 5,
//        -500, 5,
//        -250, 5,
//        -100, 5,
//        -500, 5,
//};


#define rhythm_head_length 4

const int16_t rhythm_type_size = sizeof(RhythmDataType);
// by byte
const int16_t total_length_offset = 0 * rhythm_type_size;
// by byte
const int16_t rhythm_begin_offset = rhythm_head_length; // dont need to ```rhythm_head_length * rhythm_type_size```

#define MAKE_Rhythm(name, total_length, args...)                                \
const PROGMEM RhythmDataType name[rhythm_head_length + total_length * 2] = {    \
        total_length, 0,                                                        \
        0, 0,                                                                   \
        ##args                                                                  \
}


MAKE_Rhythm(R1, 2,
            +0, 15,
            +100, 5,
);

MAKE_Rhythm(R2, 8,
            +0, 5,
            +100, 5,
            +500, 5,
            +1000, 5,
            -500, 5,
            -250, 5,
            -100, 5,
            -500, 5,
);

MAKE_Rhythm(R3, 8,
            +0, 5,
            +100, 5,
            +500, 5,
            +1000, 5,
            -500, 5,
            -250, 5,
            -100, 5,
            -500, 5,
);

const RhythmDataType RhythmTableSize = 3;
const PROGMEM RhythmDataType *const RhythmTable[RhythmTableSize] PROGMEM = {
        R1,
        R2,
        R3,
};

RhythmDataType *_getRhythmFromTable(uint16_t i) {
    return (RhythmDataType *) pgm_read_ptr(RhythmTable + i);
}

int16_t _getTotalLengthFromRhythm(RhythmDataType *r) {
//    Serial.print("_getTotalLengthFromRhythm = ");
//    for (int i = 0; i < 32; ++i) {
//        Serial.print((RhythmDataType)pgm_read_word(r + i));
//        Serial.print(" ");
//    }
//    Serial.print("\n");
    return (int16_t) pgm_read_word(r + total_length_offset);
}

RhythmDataType _getRhythmOffsetFromRhythm(RhythmDataType *r, uint16_t i) {
//    Serial.print("_getRhythmOffsetFromRhythm = ");
//    Serial.print((RhythmDataType)pgm_read_word(r + rhythm_begin_offset + (i * 2)));
//    Serial.print("\n");
    return (RhythmDataType) pgm_read_word(r + rhythm_begin_offset + (i * 2));
}

RhythmDataType _getIntervalCountFromRhythm(RhythmDataType *r, uint16_t i) {
//    Serial.print("_getIntervalCountFromRhythm = ");
//    Serial.print((RhythmDataType)pgm_read_word(r + rhythm_begin_offset + (i * 2 + 1)));
//    Serial.print("\n");
    return (RhythmDataType) pgm_read_word(r + rhythm_begin_offset + (i * 2 + 1));
}
