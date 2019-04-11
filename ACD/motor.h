/*
motor.h
class for controlling DC motor with H-bridge
*/

#ifndef MOTOR_H
#define MOTOR_H

#include "config.h"	//include the file with configurable parameters
#include <Arduino.h>

#define FORWARD 1
#define BACKWARD -1

class motor {
private:
    int motorPinA;
    int motorPinB;

public:
    //methods to control movement
    void stop();
    void drive(int direction);
    void drive(int direction, int speed);

    //Constructor. 
    motor(int pinA, int pinB);
};

#endif