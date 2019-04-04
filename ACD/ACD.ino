/*
pin connections:
horizontal motor: D5 and D6
vertical motor: D10 and D11
servos: pin 9
mic: A0
IR LED: D13
*/

#include "motor.h"
#include "mic.h"
#include <VarSpeedServo.h>

VarSpeedServo myservo;    // create servo object to control a servo 
void openCollection();
void closeCollection();

// the setup function runs once when you press reset or power the board
void setup() {

    setupMic();
    myservo.attach(9);  // attaches the servo on pin 9
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}

void openCollection() {
    myservo.write(100, 30, true);        // move to 100 degrees, use a speed of 30, wait until move is complete
}
void closeCollection() {
    myservo.write(0, 30, true);        // move to 0 degrees, use a speed of 30, wait until move is complete
}


