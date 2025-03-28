#include <Arduino.h>
#include "SerialComms.h"

#define BAUD_RATE 115200
#define PIN_PWM_OUT_L 4
#define PIN_DIR_OUT_L 5
#define PIN_PWM_OUT_R 14
#define PIN_DIR_OUT_R 12

// Define the state array: state[0] holds the PWM value, state[1] holds the direction.
int state[2] = {0, 0};

void setup() {
  // Set output pins
  pinMode(PIN_PWM_OUT_L, OUTPUT);
  pinMode(PIN_DIR_OUT_L, OUTPUT);

  pinMode(PIN_PWM_OUT_R, OUTPUT);
  pinMode(PIN_DIR_OUT_R, OUTPUT);

  // Initialize outputs
  analogWrite(PIN_PWM_OUT_L, 0);
  digitalWrite(PIN_DIR_OUT_L, 0);

  analogWrite(PIN_PWM_OUT_R, 0);
  digitalWrite(PIN_DIR_OUT_R, 1);

  // Initialize serial port
  Serial.begin(BAUD_RATE);
  Serial.println("---- Program Started ----");
}

void loop() {
  // Read serial commands
  ReadFromSerial();
  
  // Update outputs based on state
  analogWrite(PIN_PWM_OUT_L, state[0]*1.2);

  if(state[1]) digitalWrite(PIN_DIR_OUT_L, 0);
  else digitalWrite(PIN_DIR_OUT_L, 1);

  analogWrite(PIN_PWM_OUT_R, state[0]);
  digitalWrite(PIN_DIR_OUT_R, state[1]);
}
