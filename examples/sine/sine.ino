#include "ShiftRegisterPWM.h"

ShiftRegisterPWM shiftRegisterPWM(1, 16);

void setup()
{
  pinMode(2, OUTPUT); // sr data pin
  pinMode(3, OUTPUT); // sr clock pin
  pinMode(4, OUTPUT); // sr latch pin
  
  shiftRegisterPWM.interrupt(ShiftRegisterPWM::UpdateFrequency::SuperFast);
}

void loop()
{
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t val = (uint8_t)(((float)sin(millis() / 150.0 + i / 8.0 * 2.0 * PI) + 1) * 128);
    shiftRegisterPWM.set(i, val);
  }
}
