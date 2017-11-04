/**
  * Library for PWM control of the 74HC595 shift register.
  * Created by Timo Denk (www.timodenk.com), 2017.
  * Additional information is available at https://timodenk.com/blog/shiftregister-pwm-library/
  * Released into the public domain.
  */


// Definition for that latch clock to be connected to pin 8
// See https://www.arduino.cc/en/Reference/PortManipulation
#define ShiftRegisterPWM_LATCH_PORT PORTB
#define ShiftRegisterPWM_LATCH_MASK 1
#include "ShiftRegisterPWM.h"

void setup() {
  pinMode(2, OUTPUT); // data
  pinMode(3, OUTPUT); // clock
  pinMode(8, OUTPUT); // latch pin has changed!
}

void loop()
{
  ShiftRegisterPWM sr(1, 255);

  sr.interrupt(ShiftRegisterPWM::UpdateFrequency::Fast);
  while (true) {
    for (uint8_t i = 0; i < 8; i++) {
      uint8_t val = (uint8_t)(((float)sin(millis() / 150.0 + i / 8.0 * 2.0 * PI) + 1) * 128);
      sr.set(i, val);
    }
  }
}