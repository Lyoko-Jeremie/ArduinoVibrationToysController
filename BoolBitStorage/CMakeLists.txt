# define the lib BoolBitStorage
# follow this document https://github.com/queezythegreat/arduino-cmake#creating-libraries
generate_arduino_library(BoolBitStorage_lib
        HDRS BoolBitStorage.h
        SRCS BoolBitStorage.cpp
        BOARD uno)
