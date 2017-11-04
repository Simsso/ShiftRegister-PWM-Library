/**
  * Library for PWM control of the 74HC595 shift register.
  * Created by Timo Denk (www.timodenk.com), 2017.
  * Additional information is available at https://timodenk.com/blog/shiftregister-pwm-library/
  * Released into the public domain.
  */


#include "ShiftRegisterPWM.h"

ShiftRegisterPWM sr(1, 16);

void setup()
{
  pinMode(2, OUTPUT); // sr data pin
  pinMode(3, OUTPUT); // sr clock pin
  pinMode(4, OUTPUT); // sr latch pin
  
  sr.interrupt(ShiftRegisterPWM::UpdateFrequency::SuperFast);
}

void loop()
{
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t val = (uint8_t)(((float)sin(millis() / 150.0 + i / 8.0 * 2.0 * PI) + 1) * 128);
    sr.set(i, val);
  }
}
