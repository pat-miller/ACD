// Class to provide control of motors via an H-bridge
#include "motor.h"

// constructor. set up pins as outputs
motor::motor(int pinA, int pinB) {
    motorPinA = pinA;
    motorPinB = pinB;
    pinMode(motorPinA, OUTPUT);
    pinMode(motorPinB, OUTPUT);
    stop();
}

// set the motor to full speed in specified direction
void motor::drive(int direction) {
    if (direction == FORWARD) {
        digitalWrite(motorPinA, HIGH);
        digitalWrite(motorPinB, LOW);
    }
    else if (direction == BACKWARD) {
        digitalWrite(motorPinA, LOW);
        digitalWrite(motorPinB, HIGH);
    }
    else {
        stop();
    }
}

// set motor to drive at a given speed and direction using pwm
void motor::drive(int direction, int speed) {
    if (direction == FORWARD) {
        analogWrite(motorPinA, speed);
        analogWrite(motorPinB, -speed);
    }
    else if (direction == BACKWARD) {
        analogWrite(motorPinA, -speed);
        analogWrite(motorPinB, speed);
    }
    else {
        stop();
    }
}

void motor::stop() {
    digitalWrite(motorPinA, LOW);
    digitalWrite(motorPinB, LOW);

}