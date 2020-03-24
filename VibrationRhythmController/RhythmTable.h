//
// Created by jeremie on 2020/03/23.
//

#ifndef H_RHYTHMTABLE_H
#define H_RHYTHMTABLE_H

typedef int16_t RhythmDataType;

extern const RhythmDataType RhythmTableSize;

enum RhythmMode {
    RhythmMode_Loop = 1,
//    RhythmMode_Reciprocate = 2,
};

enum RhythmDirection {
    RhythmDirection_Forward = 1,
    RhythmDirection_Reverse = 2,
    RhythmDirection_Random = 3,
};

#endif //H_RHYTHMTABLE_H
