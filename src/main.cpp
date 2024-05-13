/*

Bluebird data physicalization proof-of-concept.
Controls 4 servo motors using a PCA9685 PWM controller.

---
Master: Master Information Studies: Information Systems (track)
University: University of Amsterdam (UvA)
Institute: Informatics Institute
Faculty: Faculty of Science (FNWI)
Research Group: Digital Interactions Lab (DIL)
Student: BSc Danny de Vries (14495643)
Supervisor: Dr. H. (Hamed) Seiied Alavi PhD
---
*/

/* Include Arduino Framework (built-in) */
#include <Arduino.h>

/* Incude for I2C communications (built-in) */
#include <Wire.h>
#include <SPI.h>

/* Incude PWM library */
#include <Adafruit_PWMServoDriver.h>

/* Set the PWM I2C address (default) */
#define PWM_ADDRESS 0x40

/* PWM frequency for the PCA9685 in Hz */
#define PWM_FREQUENCY 50

/* Connect the servo's on the PWM ports on the board */
#define MOTOR_ONE 0

/* Rotation in milliseconds (speed for rotation and 'growing' factor) */
#define MOVE_TIME 2000

/* Define servo minimum and maximum pulse width in microseconds */
#define SERVO_MIN_PULSE_WIDTH 0
#define SERVO_MAX_PULSE_WIDTH 500

/* Initialize the library with the default PWM address */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PWM_ADDRESS);

/*
  MG90S 360degree continous servo motor pulse widths for rotation
  Figured out by trial and error and changing values.
  Adjust accordingly based on type of servo and manufacturer spec.
  CAVIAT: Have no specified angles like servo motors so wind-up is based on time instead of angle.
 */

#define PULSE_WIDTH_NEUTRAL 325          // Neutral position 'Stops the servo motor'
#define PULSE_WIDTH_CLOCKWISE 311        // Slowest rotation for clockwise
#define PULSE_WIDTH_COUNTERCLOCKWISE 336 // Slowest rotation for counterclockwise

/* Set-up function, only runs on start-up */
void setup()
{

  /* Set the data rate (baud) for serial monitor */
  Serial.begin(9600);

  /* Initialize PWM driver on the specified frequency */
  pwm.begin();
  pwm.setPWMFreq(PWM_FREQUENCY);
}

/* Sets the servo motor pulse width to neutral 'stopping the servo' */
void stopServo(int motorNumber)
{
  pwm.setPWM(motorNumber, 0, PULSE_WIDTH_NEUTRAL);
}

/* Sets the servo motor pulse to move clockwise for 2 seconds 'growing the string' */
void moveClockwise(int motorNumber)
{
  unsigned long startTime = millis(); // Record the start time
  while (millis() - startTime < MOVE_TIME)
  {
    pwm.setPWM(motorNumber, 0, PULSE_WIDTH_CLOCKWISE); // Spin clockwise for 2 seconds
  }
  stopServo(motorNumber);
}

/* Sets the servo motor pulse to move counterclockwise for 2 seconds 'shrinking the string' */
void moveCounterClockwise(int motorNumber)
{
  unsigned long startTime = millis(); // Record the start time
  while (millis() - startTime < MOVE_TIME)
  {
    pwm.setPWM(motorNumber, 0, PULSE_WIDTH_COUNTERCLOCKWISE); // Spin counter clockwise for 2 seconds
  }
  stopServo(motorNumber);
}

void loop()
{
  moveClockwise(0);        // Move clockwise for 2 seconds and stop
  delay(1000);            // Delay for 1 second between movements
  moveCounterClockwise(0); // Move counterclockwise for 2 seconds and stop
  delay(1000);            // Delay for 1 second between movements
}