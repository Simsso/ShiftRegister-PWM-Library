/*
  ShiftRegisterPWM.h - Library for easy control of the 74HC595 shift register.
  Created by Timo Denk (www.timodenk.com), Jun 2017.
  Additional information is available at https://timodenk.com/blog/shiftregister-pwm-library/
  Released into the public domain.
*/


#ifndef ShiftRegisterPWM_h
#define ShiftRegisterPWM_h


#include <stdlib.h>
#include <avr/interrupt.h>


#ifndef ShiftRegisterPWM_DATA_PORT
  #define ShiftRegisterPWM_DATA_PORT PORTD
#endif
#ifndef ShiftRegisterPWM_DATA_MASK
  #define ShiftRegisterPWM_DATA_MASK 0B00000100
#endif

#ifndef ShiftRegisterPWM_CLOCK_PORT
  #define ShiftRegisterPWM_CLOCK_PORT PORTD
#endif
#ifndef ShiftRegisterPWM_CLOCK_MASK
  #define ShiftRegisterPWM_CLOCK_MASK 0B00001000
#endif

#ifndef ShiftRegisterPWM_LATCH_PORT
  #define ShiftRegisterPWM_LATCH_PORT PORTD
#endif
#ifndef ShiftRegisterPWM_LATCH_MASK
  #define ShiftRegisterPWM_LATCH_MASK 0B00010000
#endif

#define ShiftRegisterPWM_setDataPin() ShiftRegisterPWM_DATA_PORT |= ShiftRegisterPWM_DATA_MASK;
#define ShiftRegisterPWM_clearDataPin() ShiftRegisterPWM_DATA_PORT &= ~ShiftRegisterPWM_DATA_MASK;
#define ShiftRegisterPWM_toggleClockPinTwice() ShiftRegisterPWM_CLOCK_PORT ^= ShiftRegisterPWM_CLOCK_MASK; ShiftRegisterPWM_CLOCK_PORT ^= ShiftRegisterPWM_CLOCK_MASK
#define ShiftRegisterPWM_toggleLatchPinTwice() ShiftRegisterPWM_LATCH_PORT ^= ShiftRegisterPWM_LATCH_MASK; ShiftRegisterPWM_LATCH_PORT ^= ShiftRegisterPWM_LATCH_MASK


class ShiftRegisterPWM
{
public:
  enum UpdateFrequency {
    VerySlow, // 6,400 Hz interrupt
    Slow, // 12,800 Hz interrupt
    Medium, // 25,600 Hz interrupt
    Fast, // 35,714 Hz interrupt
    SuperFast // 51,281 Hz interrupt
  };

  ShiftRegisterPWM(uint8_t, uint8_t);
  void set(uint8_t, uint8_t); // set PWM value of a pin

  void update();
  void interrupt() const;
  void interrupt(UpdateFrequency) const;

  static ShiftRegisterPWM *singleton; // used inside the ISR

private:
  uint8_t shiftRegisterCount = 1; // number of chained shift registers
  uint8_t resolution = 255;
  uint8_t *data; // data matrix [t + sr * resolution]
  volatile uint8_t time = 0; // time resolution of resolution steps

  void shiftOut(uint8_t) const; // high speed shift out function

  bool singleShiftRegister; // true if (shiftRegisterCount == 1)
};

#endif
