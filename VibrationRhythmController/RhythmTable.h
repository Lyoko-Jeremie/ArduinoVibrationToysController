//
// Created by jeremie on 2020/03/23.
//

#ifndef H_RHYTHMTABLE_H
#define H_RHYTHMTABLE_H

typedef int16_t RhythmDataType;

extern const RhythmDataType RhythmTableSize;
//extern const PROGMEM RhythmDataType *RhythmTable[];
extern const RhythmDataType total_length_offset;
extern const RhythmDataType rhythm_begin_offset;
extern const RhythmDataType rhythm_type_size;

RhythmDataType *getRhythmFromTable(uint16_t i);

//#define getRhythmFromTable(i) \
//(RhythmDataType *) pgm_read_ptr(RhythmTable + i)

#define getTotalLengthFromRhythm(r) \
(int16_t) pgm_read_word(r + total_length_offset)

#define getRhythmOffsetFromRhythm(r, i) \
(RhythmDataType) pgm_read_word(r + rhythm_begin_offset + (i * 2) * rhythm_type_size)

#define getIntervalCountFromRhythm(r, i) \
(RhythmDataType) pgm_read_word(r + rhythm_begin_offset + (i * 2 + 1) * rhythm_type_size)


#endif //H_RHYTHMTABLE_H
