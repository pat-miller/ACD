/*

pin connections:
horizontal motor: D5 and D6
vertical motor: A4 and A5
servos: pin 9
mic: A0
IR LED: D13
*/

#define IR_LED_PIN 2
#define STATUS_LED A2
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
#include <Servo.h>

Servo collectionServo;

motor horizontalMotor(H_MOTOR_A, H_MOTOR_B);
motor verticaMotor(V_MOTOR_A, V_MOTOR_B);

int state = FORWARD;
bool gotEgg = false;              // store whether the system is carrying an egg
bool firstMovement = true;
int toneDetetected = -1;  //store which tone range was last detected

unsigned long previousMillis = 0;
int ledState = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);

    pinMode(BOTTOM_BUTTON_PIN, INPUT);
    pinMode(TOP_BUTTON_PIN, INPUT);
    pinMode(IR_LED_PIN, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);

    //Serial.println("Start up");
    startFlashPattern();    
    digitalWrite(STATUS_LED, LOW); //off

    setupMic();
    collectionServo.attach(COLLECTION_SERVO_PIN);  // attaches the servo on pin COLLECTION_SERVO_PIN
    closeCollection();

    resetVerticalSystem();
    digitalWrite(IR_LED_PIN, HIGH);
    Serial.println("LEDs on");
}

void startFlashPattern(void) {
    for (int i = 0; i < 4; i++) {
        digitalWrite(STATUS_LED, HIGH); //on
        delay(150);
        digitalWrite(STATUS_LED, LOW); //off
        delay(150);
    }
}

void flash() {
    unsigned long currentMillis = millis();
    const long interval = 800;
    if (toneState == 0) { // low frequency, solid
        digitalWrite(STATUS_LED, HIGH); //on
    }
    else if (toneState == 1) { //high frequency
        if (currentMillis - previousMillis >= interval) {
            // save the last time you blinked the LED
            previousMillis = currentMillis;

            // if the LED is off turn it on and vice-versa:
            if (ledState == LOW) {
                ledState = HIGH;
            }
            else {
                ledState = LOW;
            }
            // set the LED with the ledState of the variable:
            digitalWrite(STATUS_LED, ledState);
        }
    }
    else {
        digitalWrite(STATUS_LED, LOW); //otherwise, off
    }
}

// the loop function runs over and over again until power down or reset
void loop() {

    if (firstMovement) {
        Serial.println("first movement sequence.");
        horizontalMove(FORWARD);
        digitalWrite(IR_LED_PIN, LOW);
        firstMovement = 0;

    }
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

    // close the blades
    if (gotEgg == 0) {
        closeCollection();
    }
}

void horizontalMove(int direction) {
    int doloop = 1;
    while (doloop == 1) {
        horizontalMotor.drive(direction, HORIZONTAL_SPEED);
        while(toneDetetected == -1){
            toneDetetected = checkForSound();
            flash();
        }
        for (int i = 0; i < 20; i++){
            toneDetetected = checkForSound();
            flash();
        }
        if (toneDetetected != -1){
            doloop = 0;
        }
    }
    horizontalMotor.stop();
}


/*
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
*/
