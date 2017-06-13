#include "ShiftRegisterPWM.h"

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop()
{
  ShiftRegisterPWM shiftRegisterPWM(1);

  shiftRegisterPWM.interrupt(ShiftRegisterPWM::UpdateFrequency::Fast_150);
  while (true) {
    for (uint8_t i = 0; i < 8; i++) {
      uint8_t val = (uint8_t)(((float)sin(millis() / 150.0 + i / 8.0 * 2.0 * PI) + 1) * 128);
      shiftRegisterPWM.set(i, val);
    }
  }
}
