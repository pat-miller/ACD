/*
motor.h
class for controlling DC motor with H-bridge
*/

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

#define FORWARD 1
#define BACKWARD -1

class motor {
private:
    // store the pins that the hbridge is connected to
    int motorPinA;
    int motorPinB;

public:
    //functions to control movement
    void stop();
    void drive(int direction);
    void drive(int direction, int speed);

    //Constructor
    motor(int pinA, int pinB);
};

#endif