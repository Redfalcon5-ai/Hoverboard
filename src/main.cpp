#include <Arduino.h>

// Constants
#define BAUD_RATE 115200
#define PIN_PWM_OUT 12
#define PIN_DIR_OUT 13
#define PIN_BRAKE_OUT 14

bool ReadFromSerial();

int pwmVal = 0;     
bool dirVal = 0;         
bool brakeVal = 0;

String _command = "";
void setup() 
{
  // Set PWM pins to outputs
  pinMode(PIN_PWM_OUT, OUTPUT);
  pinMode(PIN_DIR_OUT, OUTPUT);
  pinMode(PIN_BRAKE_OUT, OUTPUT);

  // // Initialize PWM to off
  analogWrite(PIN_PWM_OUT, 0);
  analogWrite(PIN_DIR_OUT, 1);
  analogWrite(PIN_BRAKE_OUT, 0);

  // Initialize serial port
  Serial.begin(BAUD_RATE);
  Serial.println("---- Program Started ----");
}
void loop() 
{

  ReadFromSerial();

    // Process the received command if available
  analogWrite(PIN_PWM_OUT, pwmVal);
  digitalWrite(PIN_DIR_OUT, dirVal);
  digitalWrite(PIN_BRAKE_OUT, brakeVal);

}

bool ReadFromSerial()
{    
  if (Serial.available() > 0) {
    // Read the incoming string until a newline character is received
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any leading/trailing whitespace

    // Check if the string starts with "PWM,"
    if (input.startsWith("PWM,")) {
      _command = "PWM";
      // Find the comma position
      int commaIndex = input.indexOf(',');
      if (commaIndex != -1) {
        // Extract the substring that contains the numeric value
        String valueString = input.substring(commaIndex + 1);
        pwmVal = valueString.toInt();  // Convert string to integer

        // Constrain the PWM value between 0 and 255
        pwmVal = constrain(pwmVal, 0, 255);
        brakeVal = 0;

        Serial.println("Setting PWM");

        return true;
      }
      else return false;
    } 
    else if(input.startsWith("BRAKE")){
      _command = "BRAKE";
      brakeVal = 1;
      pwmVal = 0;

      Serial.println("Braking");

      return true;
    }
    else if(input.startsWith("DIR,")){
      _command = "DIR";

      int commaIndex = input.indexOf(',');
      if (commaIndex != -1) {
        // Extract the substring that contains the numeric value
        String valueString = input.substring(commaIndex + 1);
        dirVal = valueString.toInt();  // Convert string to integer

        Serial.println("Setting Direction");

        return true;
      }
      else return false;
    }
    else {
      // If the received string does not match the expected command, print an error message.
      Serial.println("Invalid command");
      return false;
    }
  }
  return false;
}