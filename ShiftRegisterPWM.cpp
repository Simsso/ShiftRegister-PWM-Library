#include "ShiftRegisterPWM.h"

#include "Arduino.h"

ShiftRegisterPWM *ShiftRegisterPWM::singleton = NULL;

ShiftRegisterPWM::ShiftRegisterPWM(uint8_t shiftRegisterCount)
{
  this->shiftRegisterCount = shiftRegisterCount;

  // init data
  // two-dimensional array: first dimension time, second dimension shift register bytes
  // data[t + sr * 256]
  this->data = (uint8_t *) malloc(256 * shiftRegisterCount * sizeof(uint8_t));
  for (uint16_t t = 0; t < 256; ++t)
  {
    for (uint16_t i = 0; i < this->shiftRegisterCount; ++i)
    {
      this->data[t + i * 256] = 0;
    }
  }

  ShiftRegisterPWM::singleton = this;
};

void ShiftRegisterPWM::set(uint8_t pin, uint8_t value) 
{
  uint8_t shiftRegister = pin / 8;
  for (uint16_t t = 0; t < 256; ++t)
  {
    // set (pin % 8)th bit to (value > t)
    this->data[t + shiftRegister * 256] ^= (-(value > t) ^ this->data[t + shiftRegister * 256]) & (1 << (pin % 8));
  }
};

void ShiftRegisterPWM::update()
{
  for (uint16_t i = 0; i < this->shiftRegisterCount; ++i)
  {
    this->shiftOut(this->data[time + i * 256]);
  }
  ShiftRegisterPWM_toggleLatchPin();
  ShiftRegisterPWM_toggleLatchPin();
  this->time++; // update time step
};

void ShiftRegisterPWM::interrupt()
{
  this->interrupt(ShiftRegisterPWM::UpdateFrequency::Medium_100);
};

void ShiftRegisterPWM::interrupt(UpdateFrequency updateFrequency)
{
  cli(); // disable interrupts
  
  // reset
  TCCR1A = 0; // set TCCR1A register to 0
  TCCR1B = 0; // set TCCR1B register to 0
  TCNT1  = 0; // reset counter value

  switch (updateFrequency) {
    case VerySlow_25: // exactly 6,400 Hz interrupt frequency
      OCR1A = 2499; // compare match register
      TCCR1B |= (1 << CS10); // prescaler 1
      break;
      
    case Slow_50: // exactly 12,800 Hz interrupt frequency
      OCR1A = 1249; // compare match register
      TCCR1B |= (1 << CS10); // prescaler 1
      break;
      
    case Fast_150: // aprox. 35,714 Hz interrupt frequency
      OCR1A = 55; // compare match register
      TCCR1B |= (1 << CS11); // prescaler 8
      break;
      
    case SuperFast_200: // approx. 51,281.5 Hz interrupt frequency
      OCR1A = 311; // compare match register
      TCCR1B |= (1 << CS10); // prescaler 1
      break;
      
    case Medium_100: // exactly 25,600 Hz interrupt frequency
    default:
      OCR1A = 624; // compare match register
      TCCR1B |= (1 << CS10); // prescaler 1
      break;
  }
  
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  
  sei(); // allow interrupts
}


ISR(TIMER1_COMPA_vect) { // function which will be called when an interrupt occurs at timer 1
  ShiftRegisterPWM::singleton->update();
};

// shift out function
// performance optimized
void ShiftRegisterPWM::shiftOut(uint8_t data) const
{
  ShiftRegisterPWM_setDataPin();
  uint8_t setRegister = ShiftRegisterPWM_DATA_PORT;
  ShiftRegisterPWM_clearDataPin();
  uint8_t clearedRegister = ShiftRegisterPWM_DATA_PORT;
  
  // unrolled for loop
  // bit 0 (LSB)
  ShiftRegisterPWM_DATA_PORT = (data & 0B00000001) ? setRegister : clearedRegister;
  ShiftRegisterPWM_toggleClockPin();
  ShiftRegisterPWM_toggleClockPin();

  // bit 1
  ShiftRegisterPWM_DATA_PORT = (data & 0B00000010) ? setRegister : clearedRegister;
  ShiftRegisterPWM_toggleClockPin();
  ShiftRegisterPWM_toggleClockPin();

  // bit 2
  ShiftRegisterPWM_DATA_PORT = (data & 0B00000100) ? setRegister : clearedRegister;
  ShiftRegisterPWM_toggleClockPin();
  ShiftRegisterPWM_toggleClockPin();

  // bit 3
  ShiftRegisterPWM_DATA_PORT = (data & 0B00001000) ? setRegister : clearedRegister;
  ShiftRegisterPWM_toggleClockPin();
  ShiftRegisterPWM_toggleClockPin();

  // bit 4
  ShiftRegisterPWM_DATA_PORT = (data & 0B00010000) ? setRegister : clearedRegister;
  ShiftRegisterPWM_toggleClockPin();
  ShiftRegisterPWM_toggleClockPin();

  // bit 5
  ShiftRegisterPWM_DATA_PORT = (data & 0B00100000) ? setRegister : clearedRegister;
  ShiftRegisterPWM_toggleClockPin();
  ShiftRegisterPWM_toggleClockPin();

  // bit 6
  ShiftRegisterPWM_DATA_PORT = (data & 0B01000000) ? setRegister : clearedRegister;
  ShiftRegisterPWM_toggleClockPin();
  ShiftRegisterPWM_toggleClockPin();

  // bit 7
  ShiftRegisterPWM_DATA_PORT = (data & 0B10000000) ? setRegister : clearedRegister;
  ShiftRegisterPWM_toggleClockPin();
  ShiftRegisterPWM_toggleClockPin();
};
