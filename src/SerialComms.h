#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <Arduino.h>

// The state array is declared here as external so that it can be shared between files.
// state[0] is the PWM value and state[1] is the direction.
extern int state[2];

bool ReadFromSerial();
bool isNumeric(const String &str);

#endif
