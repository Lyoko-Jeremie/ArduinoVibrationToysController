//
// Created by jeremie on 2020/03/23.
//

#ifndef H_RHYTHMTABLE_INTERNAL_H
#define H_RHYTHMTABLE_INTERNAL_H

#include "RhythmTable.h"

//extern const PROGMEM RhythmDataType *RhythmTable[];
extern const RhythmDataType total_length_offset;
extern const RhythmDataType rhythm_begin_offset;
extern const RhythmDataType rhythm_type_size;

RhythmDataType *_getRhythmFromTable(uint16_t i);

int16_t _getTotalLengthFromRhythm(RhythmDataType *r);

RhythmDataType _getRhythmOffsetFromRhythm(RhythmDataType *r, uint16_t i);

RhythmDataType _getIntervalCountFromRhythm(RhythmDataType *r, uint16_t i);


#define getRhythmFromTable(i) \
_getRhythmFromTable(i)
//(RhythmDataType *) pgm_read_ptr(RhythmTable + i)

#define getTotalLengthFromRhythm(r) \
_getTotalLengthFromRhythm(r)
//(int16_t) pgm_read_word(r + total_length_offset)

#define getRhythmOffsetFromRhythm(r, i) \
_getRhythmOffsetFromRhythm(r, i)
//(RhythmDataType) pgm_read_word(r + rhythm_begin_offset + (i * 2) * rhythm_type_size)

#define getIntervalCountFromRhythm(r, i) \
_getIntervalCountFromRhythm(r, i)
//(RhythmDataType) pgm_read_word(r + rhythm_begin_offset + (i * 2 + 1) * rhythm_type_size)


#endif //H_RHYTHMTABLE_INTERNAL_H
