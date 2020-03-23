#include <Arduino.h>

#include "4x4MatrixKeyboard/4x4MatrixKeyboard.h"
#include "VibrationRhythmController/VibrationRhythmController.h"

// test PWM

#include <Servo.h>


Servo servos[3];

void key_test(uint8_t edge, uint8_t keynum);

void setup() {

    Serial.begin(9600);
    testCheckRhythmTable();
    return;


    initPortState();
    setCallBackFunction(0, key_test);
    setCallBackFunction(0, key_test);
    setCallBackFunction(1, key_test);
    setCallBackFunction(2, key_test);
    setCallBackFunction(3, key_test);
    setCallBackFunction(4, key_test);
    setCallBackFunction(5, key_test);
    setCallBackFunction(6, key_test);
    setCallBackFunction(7, key_test);
    setCallBackFunction(8, key_test);
    setCallBackFunction(9, key_test);
    setCallBackFunction(10, key_test);
    setCallBackFunction(11, key_test);
    setCallBackFunction(12, key_test);
    setCallBackFunction(13, key_test);
    setCallBackFunction(14, key_test);
    setCallBackFunction(15, key_test);

    servos[0].attach(10);
    servos[1].attach(11);
    servos[2].attach(12);
    Serial.begin(9600);
}

void sleep(unsigned long ms) {
    ScanKeyAndCallKeyCallBackFunction();
    delay(ms);
}

void mode1();

void mode2();

void mode3();

void loop() {

//    delay(5000);
    return;

//    key_test(5, 20);
    ScanKeyAndCallKeyCallBackFunction();



//    Serial.print("millis = ");
//    Serial.print(millis());
//    Serial.print("\n");
//    mode3();
//    mode1();
//    mode2();
}

void key_test(uint8_t edge, uint8_t keynum) {
    Serial.print("key_test = ");
    Serial.print(keynum + 1); //
    Serial.print(" on ");
    Serial.print(edge);
    Serial.print("\n");
}

void mode1() {

    for (int j = 0; j < 10; ++j) {
        servos[0].writeMicroseconds(1500);
        servos[1].writeMicroseconds(1500);
        servos[2].writeMicroseconds(1500);
        sleep(50);
    }
    for (int i = 0; i < 100; ++i) {
        int val = map(i, 0, 100, 1000, 2000);
        servos[0].writeMicroseconds(val);
        servos[1].writeMicroseconds(val);
        servos[2].writeMicroseconds(val);
        sleep(15);
    }
    for (int j = 0; j < 10; ++j) {
        servos[0].writeMicroseconds(1500);
        servos[1].writeMicroseconds(1500);
        servos[2].writeMicroseconds(1500);
        sleep(50);
    }

}

void mode2() {

    for (int j = 0; j < 10; ++j) {
        servos[0].writeMicroseconds(1500);
        servos[1].writeMicroseconds(1500);
        servos[2].writeMicroseconds(1500);
        sleep(50);
    }
    for (int i = 0; i < 100; ++i) {
        int val = map(i, 0, 100, 2000, 1000);
        servos[0].writeMicroseconds(val);
        servos[1].writeMicroseconds(val);
        servos[2].writeMicroseconds(val);
        sleep(15);
    }
    for (int j = 0; j < 10; ++j) {
        servos[0].writeMicroseconds(1500);
        servos[1].writeMicroseconds(1500);
        servos[2].writeMicroseconds(1500);
        sleep(50);
    }

}


void mode3() {

    for (int j = 0; j < 10; ++j) {
        servos[0].writeMicroseconds(1500);
        servos[1].writeMicroseconds(1500);
        servos[2].writeMicroseconds(1500);
        sleep(50);
    }
    int val;
    for (int i = 0; i < 100; ++i) {
        val = map(i, 0, 100, 2000, 1000);
        servos[0].writeMicroseconds(val);
        val = map(i, 0, 100, 1000, 2000);
        servos[1].writeMicroseconds(val);
        if (i < (100 / 2)) {
            val = map(i * 2, 0, 100, 1000, 2000);
        } else {
            val = map(100 - (i - (100 / 2)) * 2, 0, 100, 1000, 2000);
        }
        servos[2].writeMicroseconds(val);
        if (i == 0) {
            sleep(100);
        } else {
            sleep(15);
        }
    }
    for (int j = 0; j < 10; ++j) {
        servos[0].writeMicroseconds(1500);
        servos[1].writeMicroseconds(1500);
        servos[2].writeMicroseconds(1500);
        sleep(50);
    }

}






