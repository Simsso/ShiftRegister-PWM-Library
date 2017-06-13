#include "ShiftRegisterPWM.h"

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  ShiftRegisterPWM shiftRegisterPWM(1);

  shiftRegisterPWM.set(0, 10);
  shiftRegisterPWM.set(1, 20);
  shiftRegisterPWM.set(2, 10);
  shiftRegisterPWM.set(3, 9);
  shiftRegisterPWM.set(4, 6);
  shiftRegisterPWM.set(5, 4);
  shiftRegisterPWM.set(6, 2);
  shiftRegisterPWM.set(7, 1);
  shiftRegisterPWM.interrupt();
  while (true) {
    shiftRegisterPWM.set(0, (uint8_t)(millis() / 10));
    Serial.println("hi there, how are you doing?");
  }
}
