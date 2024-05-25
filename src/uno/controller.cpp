/*

Title: Bluebird data physicalization proof-of-concept.
Description: Controls six 360 degree servo motors rotating clockwise or counterclockwise
based on simulated CO2 concentrations and movement time between difference of current and previous values.

Hardware: Arduino Uno
Shield: PCA9685 PWM controller.
Components: MG90S Servo Motors

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
#define MOTOR_TWO 1
#define MOTOR_THREE 2
#define MOTOR_FOUR 3
#define MOTOR_FIVE 4
#define MOTOR_SIX 5

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
#define PULSE_WIDTH_CLOCKWISE 305        // Slowest rotation for clockwise
#define PULSE_WIDTH_COUNTERCLOCKWISE 336 // Slowest rotation for counterclockwise

/* Variables to store timer functions */
unsigned long previousMillis = 0;     // Variable to store the last time the function was called
const unsigned long interval = 10000; // Interval in milliseconds (10 seconds)

/* Variables to store the CO2 concentration levels*/
int currentNumber;    // Global variable to store the current level
int previousNumber;   // Global variable to store the previous level
int differenceNumber; // Global variable to store the difference between the current and previous numbers
int moveTime;

/* String of UART communication*/
String receivedData = "";
float co2Concentration = 0.0;

/* Set-up function, only runs on start-up */
void setup()
{

  /* Set the data rate (baud) for serial monitor */
  Serial.begin(4800);

  /* Initialize PWM driver on the specified frequency */
  pwm.begin();
  pwm.setPWMFreq(PWM_FREQUENCY);
}

/* Function that generates 'random' CO2 concentration levels each 5 seconds */
void generateLevels()
{
  unsigned long currentMillis = millis(); // Get the current time

  if (currentMillis - previousMillis >= interval)
  { // Check if it's time to generate a new number
    previousNumber = currentNumber;
    currentNumber = random(500, 901);                  // Generate a random number between 500 and 900
    differenceNumber = currentNumber - previousNumber; // Calculate the difference between current and previous numbers
    previousMillis = currentMillis;                    // Save the current time for the next iteration
    moveTime = abs(differenceNumber);
  }
}

/* Sets the servo motor pulse width to neutral 'stopping the servo' */
void stopServo(int motorNumber)
{
  pwm.setPWM(motorNumber, 0, PULSE_WIDTH_NEUTRAL);
}

/* Sets the servo motor pulse to move clockwise for 2 seconds 'growing the string' */
void moveClockwise(int motorNumber)
{
  unsigned long startTime = millis();          // Record the start time
  while (millis() - startTime < moveTime * 15) // Add multiplication for move time
  {
    pwm.setPWM(motorNumber, 0, PULSE_WIDTH_CLOCKWISE); // Spin clockwise for 2 seconds
  }
  stopServo(motorNumber);
}

/* Sets the servo motor pulse to move counterclockwise for 2 seconds 'shrinking the string' */
void moveCounterClockwise(int motorNumber)
{
  unsigned long startTime = millis();          // Record the start time
  while (millis() - startTime < moveTime * 15) // Add multiplication for move time
  {
    pwm.setPWM(motorNumber, 0, PULSE_WIDTH_COUNTERCLOCKWISE); // Spin counter clockwise for 2 seconds
  }
  stopServo(motorNumber);
}

void parseCO2Concentration(String data)
{
  int index = data.indexOf("CO2 concentration: ");
  if (index >= 0)
  {
    String co2String = data.substring(index + 19); // 19 is the length of "CO2 concentration: "
    co2Concentration = co2String.toFloat();
    Serial.print("Stored CO2 concentration: ");
    Serial.println(co2Concentration);
  }
  else
  {
    Serial.println("CO2 concentration not found in the received data.");
  }
}

void loop()
{

  if (Serial.available() > 0)
  {
    String receivedData = Serial.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(receivedData);
  }

  generateLevels();
  Serial.print("Previous: ");
  Serial.println(previousNumber);
  Serial.print("Current: ");
  Serial.println(currentNumber);
  Serial.print("Difference: ");
  Serial.println(differenceNumber);
  Serial.print("Move: ");
  Serial.println(moveTime);
  delay(10000);

  if (differenceNumber > 0)
  {
    moveClockwise(MOTOR_ONE);
    moveClockwise(MOTOR_TWO);
    moveClockwise(MOTOR_THREE);
    moveClockwise(MOTOR_FOUR);
    moveClockwise(MOTOR_FIVE);
    moveClockwise(MOTOR_SIX);
    delay(1000);
  }
  else
  {
    moveCounterClockwise(MOTOR_ONE);
    moveCounterClockwise(MOTOR_TWO);
    moveCounterClockwise(MOTOR_THREE);
    moveCounterClockwise(MOTOR_FOUR);
    moveCounterClockwise(MOTOR_FIVE);
    moveCounterClockwise(MOTOR_SIX);
    delay(1000);
  }
}