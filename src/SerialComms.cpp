#include "SerialComms.h"

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
      state[0] = 0;
      Serial.println("Braking");
    } else {
      // Determine direction: positive for forward, negative for reverse
      if (speed > 0) {
        state[1] = 0;  // Forward
      } else {
        state[1] = 1;  // Reverse
        speed = -speed;  // Use absolute value for PWM mapping
      }
      
      // Map the absolute speed (0-100) to a PWM value (0-255)
      state[0] = map(speed, 0, 100, 0, 255);
      Serial.print("Setting speed: ");
      Serial.println(speed);
    }
    
    return true;
  }
  return false;
}

bool isNumeric(const String &str) {
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
