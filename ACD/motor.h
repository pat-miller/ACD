/*
motor.h
class for controlling DC motor with H-bridge
No speed control, just direction
*/

#ifndef MOTOR_H
#define MOTOR_H

#include "config.h"	//include the file with configurable parameters
#include <Arduino.h>

#define FORWARD 1
#define BACKWARD -1

class motor {
private:
                                    //ints to store the pins that the hbridge is connected to
    int motorPinA;
    int motorPinB;

public:
    //methods to control movement
    void stop();
    void drive(int direction);
    void drive(int direction, int speed);

    //Constructor. L1, L2, R1 and R2 are the pins connected to the hbridge
    motor(int pinA, int pinB);
};

#endif