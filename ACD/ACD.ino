/*
ACD code
the main code for the ACD coordination function
pin connections:
horizontal motor: D5 and D6
vertical motor: A4 and A5
servos: pin 9
mic: A0
IR LED: D13
*/

#define IR_LED_PIN 2
#define COLLECTION_SERVO_PIN 9
#define H_MOTOR_A 5
#define H_MOTOR_B 6
#define V_MOTOR_A A4
#define V_MOTOR_B A5
#define MIC_PIN A0
#define BOTTOM_BUTTON_PIN 7
#define TOP_BUTTON_PIN 8
#define HORIZONTAL_SPEED 170

#include "motor.h"
#include "mic.h"
#include <Servo.h>

// create servo object for cargo collection arms
Servo collectionServo;

// connect motors for horizontal and vertical positioning systems
motor horizontalMotor(H_MOTOR_A, H_MOTOR_B);
motor verticaMotor(V_MOTOR_A, V_MOTOR_B);

// the horizontal direction the platform is moving in
int state = FORWARD;
int gotEgg = 0;
int firstMovement = 1;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);

    pinMode(BOTTOM_BUTTON_PIN, INPUT);
    pinMode(TOP_BUTTON_PIN, INPUT);
    pinMode(IR_LED_PIN, OUTPUT);

    Serial.println("Start up");

    setupMic();
    //setup the servo
    collectionServo.attach(COLLECTION_SERVO_PIN);  // attaches the servo on pin COLLECTION_SERVO_PIN
    closeCollection();

    resetVerticalSystem();
    digitalWrite(IR_LED_PIN, HIGH);
    }

// the loop function runs over and over again until power down or reset
void loop() {

    if (firstMovement) {
        Serial.println("first movement sequence.");
        horizontalMove(FORWARD);
        digitalWrite(IR_LED_PIN, LOW);
        firstMovement = 0;
    }

    if (gotEgg == 0) {
        openCollection();
    }

    while (checkBottom() == 0) {
        verticaMotor.drive(FORWARD);
    }
    verticaMotor.stop();
    if (gotEgg == 0) {
        closeCollection();
        gotEgg = 1;
    }
    else { // open collection to release egg
        openCollection();
        delay(1000);
        gotEgg = 0;
    }

    resetVerticalSystem();

    if (gotEgg == 0) {
        closeCollection();
    }

    // move for 5 seconds to ensure we get passed the control module
    horizontalMotor.drive(state, HORIZONTAL_SPEED);
    delay(5000);
    // turn on the IR leds
    digitalWrite(IR_LED_PIN, HIGH);

    // move to next control module
    horizontalMove(state);

    digitalWrite(IR_LED_PIN, LOW);

    // change state
    state = (state == FORWARD ? BACKWARD : FORWARD);

}

void openCollection() {
    collectionServo.write(100);              // tell servo to go 100 degrees
    delay(500);
}

void closeCollection() {
    collectionServo.write(0);              // tell servo to go 0 degrees
    delay(500);
}

int checkBottom() {
    //connect button with one leg to 5V, the other to the BOTTOM_BUTTON_PIN and a 10K pulldown resistor to gnd
    if (digitalRead(BOTTOM_BUTTON_PIN) == LOW) return 1; // button is compressed and circuit is closed
    else return 0;
}

int checkTop() {
    if (digitalRead(TOP_BUTTON_PIN) == LOW) return 1;
    else return 0;
}

void resetVerticalSystem() {
    // raise till top, stop
    while (checkTop() == 0) {
        verticaMotor.drive(BACKWARD);
    }
    //back off (lower) till button is released.
    while (checkTop() == 1) {
        verticaMotor.drive(FORWARD);
    }
    delay(100); // continue a little more
    verticaMotor.stop();
    delay(500);
}

void horizontalMove(int direction) {

    int previousTrigger = 0;
    int needToMove = 1;
    int sound = 0;

    while (needToMove) {
        sound = checkForSound();

        //if its too noisy, just wait for it to be quiet
        //if (sound == -1) {
        //    horizontalMotor.stop();
        //}
        if (sound == 1 || sound == 2) {
            previousTrigger++;
            if (previousTrigger > 3) {
                Serial.println("THINK WE'VE BEEN TRIGGERED");
                //horizontalMotor.stop();
                // wait a little to be sure
                delay(300);
                // take 6 more samples, at least 4 need to be above the threshold 
                int soundCounter = 0;
                for (int i = 0; i < 6; i++) {
                    sound = checkForSound();
                    if (sound == 1 || sound == 2) {
                        soundCounter++;
                    }
                }
                if (soundCounter > 3) {
                    needToMove = 0;
                }
            }
        }
        else {
            previousTrigger = 0;
            horizontalMotor.drive(direction, HORIZONTAL_SPEED);
        }
    }
    horizontalMotor.stop();
}