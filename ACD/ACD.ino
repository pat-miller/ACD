/*
pin connections:
horizontal motor: D5 and D6
vertical motor: D10 and D11
servos: pin 9
mic: A0
IR LED: D13
*/

#define IR_LED_PIN 13
#define COLLECTION_SERVO_PIN 9
#define H_MOTOR_A 5
#define H_MOTOR_B 6
#define V_MOTOR_A 10
#define V_MOTOR_B 11
#define MIC_PIN A0
#define BOTTOM_BUTTON_PIN 2
#define TOP_BUTTON_PIN 3

#include "motor.h"
#include "mic.h"
//#include <VarSpeedServo.h>
#include <Servo.h>

//VarSpeedServo myservo;    // create servo object to control a servo 
Servo collectionServo;

motor horizontalMotor(H_MOTOR_A, H_MOTOR_B);
motor verticaMotor(V_MOTOR_A, V_MOTOR_B);


void openCollection();
void closeCollection();
int checkBottom();
int checkTop();

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);

    setupMic();
    collectionServo.attach(COLLECTION_SERVO_PIN);  // attaches the servo on pin 9
    pinMode(BOTTOM_BUTTON_PIN, INPUT);
    pinMode(TOP_BUTTON_PIN, INPUT);
    pinMode(IR_LED_PIN, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {

    /*if (checkBottom()) {
        digitalWrite(13, HIGH);
    }
    else digitalWrite(13, LOW); 
    */
    /*horizontalMotor.drive(FORWARD);
    delay(1000);
    horizontalMotor.drive(BACKWARD);
    delay(1000);
    */

    
    //if opperation == true:
    //turn on IR LEDs
    digitalWrite(IR_LED_PIN, HIGH);
    Serial.println("LEDs on");
    //move forward until we hear a sound,stop, turn off leds
    while (checkForSound() == 0) {
        horizontalMotor.drive(FORWARD);
    }
    horizontalMotor.stop();
    // open collection 
    openCollection();
    // lower until we hit the bottom, stop
    while (checkBottom() == 0) {
        verticaMotor.drive(FORWARD);
    }
    verticaMotor.stop();
    //close collection
    closeCollection();
    // raise till top, stop
    while (checkTop() == 0) {
        verticaMotor.drive(BACKWARD);
    }
    verticaMotor.stop();

    // urn on led
    // move forward till sound
    // lower to bottom
    // open
    // raise to top
    // stop

    //Serial.println(checkForSound());
    //delay(1000);
    //openCollection();
    //delay(5000);
    //closeCollection();
    //delay(5000);
    
    
}

void openCollection() {
    //myservo.write(100, 30, true);        // move to 100 degrees, use a speed of 30, wait until move is complete
    collectionServo.write(100);
}
void closeCollection() {
    //myservo.write(0, 30, true);        // move to 0 degrees, use a speed of 30, wait until move is complete
    collectionServo.write(0);
}

int checkBottom() {
    //connect button with one leg to 5V, the other to the BOTTOM_BUTTON_PIN and a 10K pulldown resistor to gnd
    if (digitalRead(BOTTOM_BUTTON_PIN) == HIGH) return 1; // button is compressed and circuit is closed
    else return 0;
}

int checkTop() {
    if (digitalRead(TOP_BUTTON_PIN) == HIGH) return 1;
    else return 0;
}

