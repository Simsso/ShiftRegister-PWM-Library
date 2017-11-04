/**
  * Library for PWM control of the 74HC595 shift register.
  * Created by Timo Denk (www.timodenk.com), 2017.
  * Additional information is available at https://timodenk.com/blog/shiftregister-pwm-library/
  * Released into the public domain.
  */


#include "ShiftRegisterPWM.h"

const int resolution = 32, numLeds = 8;
ShiftRegisterPWM sr(1, resolution);

void setup()
{
  pinMode(2, OUTPUT); // sr data pin
  pinMode(3, OUTPUT); // sr clock pin
  pinMode(4, OUTPUT); // sr latch pin

  pinMode(A0, INPUT); // potentiometer pin
  
  sr.interrupt(ShiftRegisterPWM::UpdateFrequency::SuperFast);
}

void loop()
{
  float scaled = analogRead(A0) / 1024.0f * numLeds;
  for (int i = 0; i < numLeds; i++) {
    sr.set(i, max(0, min(scaled - i, 1)) * 255);
  }
}
