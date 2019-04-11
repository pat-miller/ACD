#include "motor.h"
#include "config.h"

motor::motor(int pinA, int pinB) {
    motorPinA = pinA;
    motorPinB = pinB;
    pinMode(motorPinA, OUTPUT);
    pinMode(motorPinB, OUTPUT);
    stop();
}

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