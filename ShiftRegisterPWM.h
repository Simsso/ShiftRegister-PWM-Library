/*
  ShiftRegisterPWM.h - Library for easy control of the 74HC595 shift register.
  Created by Timo Denk (www.timodenk.com), Jun 2017.
  Additional information is available at https://shiftregister-pwm.timodenk.com/
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

#define ShiftRegisterPWM_setDataPin() ShiftRegisterPWM_DATA_PORT |= ShiftRegisterPWM_DATA_MASK
#define ShiftRegisterPWM_clearDataPin() ShiftRegisterPWM_DATA_PORT &= ~ShiftRegisterPWM_DATA_MASK
#define ShiftRegisterPWM_toggleClockPin() ShiftRegisterPWM_CLOCK_PORT ^= ShiftRegisterPWM_CLOCK_MASK
#define ShiftRegisterPWM_toggleLatchPin() ShiftRegisterPWM_LATCH_PORT ^= ShiftRegisterPWM_LATCH_MASK


class ShiftRegisterPWM
{
public:
  enum UpdateFrequency {
    VerySlow, // 25 at frequency 256
    Slow, // 50 at frequency 256
    Medium, // 100 at frequency 256
    Fast, // 150 at frequency 256
    SuperFast // 200 at frequency 256
  };

  ShiftRegisterPWM(uint8_t, uint8_t);
  void set(uint8_t pin, uint8_t value); // set PWM value of a pin

  void update();
  void interrupt() const;
  void interrupt(UpdateFrequency updateFrequency) const;

  static ShiftRegisterPWM *singleton; // used inside the ISR

private:
  uint8_t shiftRegisterCount; // number of chained shift registers
  uint8_t resolution;
  uint8_t *data; // data matrix [t + sr * resolution]
  volatile uint8_t time = 0; // time resolution of resolution steps

  void shiftOut(uint8_t data) const; // high speed shift out function
};

#endif
