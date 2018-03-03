// Setting up motors and motor shield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// TODO Configure these pins
#define PAN_MOTOR (1)           // this is the M1 pin on motor shield
#define TILT_MOTOR (2)          // this is the M2 on motor shield
#define TOSS_BUTTON_PIN 5       // spring release button
#define PAN_LEFT_BUTTON_PIN 7   // pan left button
#define PAN_RIGHT_BUTTON_PIN 6  // pan right button
#define TILT_UP_BUTTON_PIN 5    // tilt up button
#define TILT_DOWN_BUTTON_PIN 4  // tilt down button

#define PAN_PWM 255             // the PWM speed at which to pan
#define TILT_PWM 255            // the PWM speed at which to tilt
#define MIN_RANDOMIZE_TIME 150  // the minimum time to move a servo during randomization
#define MAX_RANDOMIZE_TIME 500  // the maximum time to move a servo during randomization

#define DEBOUNCE_DELAY 50       // the time between button reads

#define NUM_BUTTONS 4
short buttons[] = {
  PAN_LEFT_BUTTON_PIN,
  PAN_RIGHT_BUTTON_PIN,
  TILT_UP_BUTTON_PIN,
  TILT_DOWN_BUTTON_PIN
};
unsigned long lastDebounceTimes[NUM_BUTTONS];  // the last time the output pins were toggled
short lastButtonStates[NUM_BUTTONS];
short buttonStates[NUM_BUTTONS];

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *panMotor = AFMS.getMotor(PAN_MOTOR);
Adafruit_StepperMotor *tiltMotor = AFMS.getStepper(200, 2);
Adafruit_DCMotor *springMotor = AFMS.getMotor(TILT_MOTOR);

void setup() {
  // Initialize serial for computer debugging
  Serial.begin(9600);
  
  // Configure button pins as inputs
  for (short i = 0; i < NUM_BUTTONS; ++i) {
    pinMode(buttons[i], INPUT);
    lastButtonStates[i] = LOW;
    buttonStates[i] = LOW;
    lastDebounceTimes[i] = 0;
  }
  
  // Initialize the motors
  AFMS.begin();  // create with the default frequency 1.6KHz
  
}

void loop() {

  // Check the states of all the buttons
  for (short i = 0; i < NUM_BUTTONS; ++i) {
    int reading = digitalRead(buttons[i]);

    if (reading != lastButtonStates[i]) {
      // Button has changed state, so reset the debouncing timer for that button
      lastDebounceTimes[i] = millis();
    } else if ((millis() - lastDebounceTimes[i]) > DEBOUNCE_DELAY) {
      buttonStates[i] = reading;
    }
    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonStates[i] = reading;
  }

  // Check if 
  for (short i = 0; i < NUM_BUTTONS; ++i) {
    Adafruit_DCMotor *motor;
    short direction;
    bool usingDCMotor = true;
    
    if (buttons[i] == PAN_LEFT_BUTTON_PIN) {
      motor = panMotor;
      direction = FORWARD;
      
    } else if (buttons[i] == PAN_RIGHT_BUTTON_PIN) {
      motor = panMotor;
      direction = BACKWARD;
      
    } else if (buttons[i] == TILT_UP_BUTTON_PIN) {
      direction = FORWARD;
      usingDCMotor = false;
      
    } else if (buttons[i] == TILT_DOWN_BUTTON_PIN) {
      direction = BACKWARD;
      usingDCMotor = false;  
    }
    
    if (usingDCMotor) {
      motor->setSpeed(PAN_PWM);
      motor->run(buttonStates[i] == HIGH ? direction : RELEASE);
    } else if (buttonStates[i] == HIGH) {
      tiltMotor->step(20, direction, SINGLE);
    }
  }

}

/**
 * Pans the launcher left and right.
 * ms: the number of milliseconds to move for
 * direction: the direction to move in
 */
void pan(int ms, short direction) {
  // Move the motor
  panMotor->setSpeed(PAN_PWM);
  panMotor->run(direction);
  // Wait for ms
  delay(ms);
  // Stop the motor
  panMotor->run(RELEASE);
}

/**
 * Pans the cannon left and right.
 * ms: the number of milliseconds to move for
 * direction: the direction to move in
 */
void tilt(int ms, short direction) {
  // Move the motor
//  tiltMotor->setSpeed(TILT_PWM);
//  tiltMotor->run(direction);
  // Wait for ms
  delay(ms);
  // Stop the motor
//  tiltMotor->run(RELEASE);
}

/**
 * Tosses the bean bag by releasing the spring.
 */
void toss() {
  // TODO Release the spring
}

/**
 * Randomly tilts and pans the launcher.
 */
void randomizePosition() {
  tilt(random(MIN_RANDOMIZE_TIME, MAX_RANDOMIZE_TIME+1), getRandomDirection());
  pan(random(MIN_RANDOMIZE_TIME, MAX_RANDOMIZE_TIME+1), getRandomDirection());
}

/**
 * Returns -1 or 1 pseudo-randomly.
 */
int getRandomDirection() {
  return (int)random(0, 2) * 2 - 1;
}

