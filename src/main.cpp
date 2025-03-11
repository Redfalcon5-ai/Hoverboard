#include <Arduino.h>

// Constants
#define BAUD_RATE 115200
#define PIN_PWM_OUT 12
#define PIN_DIR_OUT 13
#define PIN_BRAKE_OUT 14

bool ReadFromSerial();
bool isNumeric(const String& str);

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



bool ReadFromSerial() {
  if (Serial.available() > 0) {
    // Read the incoming string until a newline character is received
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any leading/trailing whitespace

    // Check if the string is numeric
    if (!isNumeric(input)) {
      Serial.println("Invalid number");
      return false;
    }
    
    // Convert the input string directly to an integer (speed)
    int speed = input.toInt();

    // Constrain the speed to the range -100 to 100
    speed = constrain(speed, -100, 100);

    if (speed == 0) {
      // Zero speed means braking
      brakeVal = 1;
      pwmVal = 0;
      _command = "BRAKE";
      Serial.println("Braking");
    } else {
      brakeVal = 0;
      _command = "SPEED";

      // Determine direction: positive for forward, negative for reverse
      if (speed > 0) {
        dirVal = 0;  // Forward
      } else {
        dirVal = 1; // Reverse
        speed = -speed;  // Use absolute value for PWM mapping
      }
      
      // Map the absolute speed (0-100) to a PWM value (0-255)
      pwmVal = map(speed, 0, 100, 0, 255);
      Serial.print("Setting speed: ");
      Serial.println(speed);
    }
    
    return true;
  }
  return false;
}



bool isNumeric(const String& str) {
  String s = str;
  s.trim();  // Remove leading and trailing whitespace
  if (s.length() == 0) return false;  // Empty string isn't a valid number
  
  int start = 0;
  // Allow a leading minus sign for negative numbers
  if (s.charAt(0) == '-') {
    if (s.length() == 1) return false; // Only '-' is not a valid number
    start = 1;
  }
  
  // Check each character to ensure it's a digit
  for (int i = start; i < s.length(); i++) {
    if (!isDigit(s.charAt(i))) {
      return false;
    }
  }
  
  return true;
}
