/*
TODO:
replace with commercial hbridge, and add pwm speed conrol to make horizontal motor drive slowly
get replacement servo
add smarter sound detection code


pin connections:
horizontal motor: D5 and D6
vertical motor: A4 and A5
servos: pin 9
mic: A0
IR LED: D13
*/

#define IR_LED_PIN 4
#define COLLECTION_SERVO_PIN 9
#define H_MOTOR_A 5
#define H_MOTOR_B 6
#define V_MOTOR_A A4
#define V_MOTOR_B A5
#define MIC_PIN A0
#define BOTTOM_BUTTON_PIN 7
#define TOP_BUTTON_PIN 8
#define HORIZONTAL_SPEED 190

#include "motor.h"
#include "mic.h"
//#include <VarSpeedServo.h>
#include <Servo.h>

//VarSpeedServo myservo;    // create servo object to control a servo 
Servo collectionServo;

motor horizontalMotor(H_MOTOR_A, H_MOTOR_B);
motor verticaMotor(V_MOTOR_A, V_MOTOR_B);

int state = FORWARD;
int gotEgg = 0;
int firstMovement = 1;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);

    pinMode(BOTTOM_BUTTON_PIN, INPUT);
    pinMode(TOP_BUTTON_PIN, INPUT);
    pinMode(IR_LED_PIN, INPUT);

    Serial.println("Start up");
    /*
    for (int i = 0; i < 3; i++) {
        digitalWrite(IR_LED_PIN, HIGH);
        delay(500);
        digitalWrite(IR_LED_PIN, LOW);
        delay(500);
    }
    */

    setupMic();
    collectionServo.attach(COLLECTION_SERVO_PIN);  // attaches the servo on pin COLLECTION_SERVO_PIN
    closeCollection();

    resetVerticalSystem();
    digitalWrite(IR_LED_PIN, HIGH);
    Serial.println("LEDs on");
    }

// the loop function runs over and over again until power down or reset
void loop() {

    if (firstMovement) {
        Serial.println("first movement sequence.");
        horizontalMove(FORWARD);
        digitalWrite(IR_LED_PIN, LOW);
        firstMovement = 0;

    }
    Serial.print("gotEgg = ");
    Serial.println(gotEgg);
    Serial.print("state: ");
    Serial.println(state);
        // open collection 
    if (gotEgg == 0) {
        openCollection();
    }
    // lower until we hit the bottom, stop
    while (checkBottom() == 0) {
        verticaMotor.drive(FORWARD);
    }
    verticaMotor.stop();
    //close collection to collect egg
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

    // Turn on led
    digitalWrite(IR_LED_PIN, HIGH);
    //move forward until we hear a sound,stop, turn off leds
    
    horizontalMotor.drive(state, HORIZONTAL_SPEED);
    delay(5000);
    horizontalMove(state);

    digitalWrite(IR_LED_PIN, LOW);
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
    while (checkTop() == 1) {
        verticaMotor.drive(FORWARD);
    }
    delay(100);
    verticaMotor.stop();
    Serial.println("vertical system is all reset and ready");
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
            Serial.println(previousTrigger);
            if (previousTrigger > 3) {
                Serial.println("THINK WE'VE BEEN TRIGGERED");
                delay(300);
                int soundCounter = 0;
                for (int i = 0; i < 5; i++) {
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
            horizontalMotor.drive(direction,HORIZONTAL_SPEED);
        }
    }
    horizontalMotor.stop();
}