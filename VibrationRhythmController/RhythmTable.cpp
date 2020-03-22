//
// Created by jeremie on 2020/03/23.
//

#include <Arduino.h>

#include "RhythmTable.h"

// the data example
//const PROGMEM int16_t Rhythm[2 + 8 * 2] = {
//        // total length, reserve,      <==== the Rhythm info head
//        8, 0,
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


const int16_t rhythm_type_size = sizeof(RhythmDataType);
// by byte
const int16_t total_length_offset = 0 * rhythm_type_size;
// by byte
const int16_t rhythm_begin_offset = 2 * rhythm_type_size;

#define MAKE_Rhythm(name, total_length, args...)                            \
const PROGMEM RhythmDataType name[2 + total_length * 2] = {                        \
        total_length, 0,                                                    \
        ##args                                                              \
}


MAKE_Rhythm(R1, 2,
            +0, 15,
            +100, 5,
);

const RhythmDataType RhythmTableSize = 5;
const PROGMEM RhythmDataType *const RhythmTable[5] = {
        R1,
        R1,
        R1,
        R1,
        R1,
};

RhythmDataType *getRhythmFromTable(uint16_t i) {
    return (RhythmDataType *) pgm_read_ptr(RhythmTable + i);
}
