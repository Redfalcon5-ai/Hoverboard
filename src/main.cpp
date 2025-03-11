#include <Arduino.h>
#include "SerialComms.h"

#define BAUD_RATE 115200
#define PIN_PWM_OUT 12
#define PIN_DIR_OUT 13

// Define the state array: state[0] holds the PWM value, state[1] holds the direction.
int state[2] = {0, 0};

void setup() {
  // Set output pins
  pinMode(PIN_PWM_OUT, OUTPUT);
  pinMode(PIN_DIR_OUT, OUTPUT);

  // Initialize outputs
  analogWrite(PIN_PWM_OUT, 0);
  digitalWrite(PIN_DIR_OUT, 1);

  // Initialize serial port
  Serial.begin(BAUD_RATE);
  Serial.println("---- Program Started ----");
}

void loop() {
  // Read serial commands
  ReadFromSerial();
  
  // Update outputs based on state
  analogWrite(PIN_PWM_OUT, state[0]);
  digitalWrite(PIN_DIR_OUT, state[1]);
}
