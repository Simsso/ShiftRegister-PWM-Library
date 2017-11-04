/*
  Library for PWM control of the 74HC595 shift register.
  Created by Timo Denk (www.timodenk.com), 2017.
  Additional information is available at https://timodenk.com/blog/shiftregister-pwm-library/
  Released into the public domain.
*/

#include "ShiftRegisterPWM.h"

// number of chained shift register and PWM resolution (16 different levels)
ShiftRegisterPWM sr(1, 16);

void setup()
{
  pinMode(2, OUTPUT); // sr data pin
  pinMode(3, OUTPUT); // sr clock pin
  pinMode(4, OUTPUT); // sr latch pin

  // instead of using the following function call, we set up the interrupt manually
  //sr.interrupt(ShiftRegisterPWM::UpdateFrequency::SuperFast);
  
  cli(); // disable interrupts 

  // timer2
  TCCR2A = 0; // set TCCR2A register to 0
  TCCR2B = 0; // set TCCR2B register to 0
  TCNT2  = 0; // set counter value to 0

  // With a prescaler of 1:256 this corresponds to an interrupt frequency of 1600 Hz.
  // According to the timer interrupt calculator https://tic.simsso.de/
  OCR2A = 38; // set compare match register
  
  TCCR2B |= (1 << CS22); // Set CS21 bits for 1:256 prescaler

  TCCR2A |= (1 << WGM21); // turn on CTC mode
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt
  
  sei(); // allow interrupts
}


// timer 2 interrupt service routine (will be called at 1600 Hz)
ISR(TIMER2_COMPA_vect) {
  // call the update method manually
  // must be called with a fixed frequency (therefore within the ISR)
  cli();
  sr.update();
  sei();
}


void loop()
{
  // output sine pattern
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t val = (uint8_t)(((float)sin(millis() / 150.0 + i / 8.0 * 2.0 * PI) + 1) * 128);
    sr.set(i, val);
  }
}
