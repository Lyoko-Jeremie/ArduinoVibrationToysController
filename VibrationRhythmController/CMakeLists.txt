# define the lib VibrationRhythmController
# follow this document https://github.com/queezythegreat/arduino-cmake#creating-libraries
generate_arduino_library(VibrationRhythmController_lib
        SRCS VibrationRhythmController.cpp RhythmTable.cpp
        HDRS VibrationRhythmController.h RhythmTable.h RhythmTable_intenal.h
        BOARD uno)
