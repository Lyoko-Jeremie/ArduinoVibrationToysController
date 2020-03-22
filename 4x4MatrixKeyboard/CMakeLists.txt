# define the lib 4x4MatrixKeyboard
# follow this document https://github.com/queezythegreat/arduino-cmake#creating-libraries
generate_arduino_library(4x4MatrixKeyboard_lib
        SRCS  4x4MatrixKeyboard.cpp
        HDRS  4x4MatrixKeyboard.h
        BOARD uno)
