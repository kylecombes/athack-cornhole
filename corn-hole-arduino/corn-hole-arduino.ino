// Setting up motors and motor shield
#include <Adafruit_MotorShield.h>

// TODO Configure these pins
#define PAN_MOTOR (4)           // this is the M1 pin on motor shield
#define TILT_MOTOR (3)          // this is the M2 on motor shield
#define TOSS_BUTTON_PIN 5       // spring release button
#define PAN_LEFT_BUTTON_PIN 5   // pan left button
#define PAN_RIGHT_BUTTON_PIN 5  // pan right button
#define TILT_UP_BUTTON_PIN 5    // tilt up button
#define TILT_DOWN_BUTTON_PIN 5  // tilt down button

#define PAN_PWM 100         // the PWM speed at which to pan
#define TILT_PWM 100        // the PWM speed at which to tilt
#define MIN_RANDOMIZE_TIME  // the minimum time to move a servo during randomization
#define MAX_RANDOMIZE_TIME  // the maximum time to move a servo during randomization

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *panMotor = AFMS.getMotor(PAN_MOTOR);
Adafruit_DCMotor *tiltMotor = AFMS.getMotor(TILT_MOTOR);

void setup() {
  // Initialize serial for computer debugging
  Serial.begin(9600);
  
  // Initialize the motors
  AFMS.begin();  // create with the default frequency 1.6KHz
  
  // TODO Init buttons
  
}

void loop() {
  // TODO Check for button presses
}

/**
 * Pans the launcher left and right.
 * ms: the number of milliseconds to move for
 * direction: the direction to move in (1 for left/counter-clockwise, -1 for right/clockwise)
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
 * direction: the direction to move in (1 for up, -1 for down)
 */
void tilt(int ms, short direction) {
  // Move the motor
  tiltMotor->setSpeed(TILT_PWM);
  tiltMotor->run(direction);
  // Wait for ms
  delay(ms);
  // Stop the motor
  tiltMotor->run(RELEASE);
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

