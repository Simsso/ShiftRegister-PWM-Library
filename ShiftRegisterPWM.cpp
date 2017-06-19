#include "ShiftRegisterPWM.h"

ShiftRegisterPWM *ShiftRegisterPWM::singleton = NULL;

ShiftRegisterPWM::ShiftRegisterPWM(uint8_t shiftRegisterCount, uint8_t resolution)
{
  this->shiftRegisterCount = shiftRegisterCount;
  this->resolution = resolution;

  // init data
  // two-dimensional array: first dimension time, second dimension shift register bytes
  // data[t + sr * resolution]
  this->data = (uint8_t *) malloc(resolution * shiftRegisterCount * sizeof(uint8_t));
  for (int t = 0; t < resolution; ++t)
  {
    for (int i = 0; i < this->shiftRegisterCount; ++i)
    {
      this->data[t + i * resolution] = 0;
    }
  }

  ShiftRegisterPWM::singleton = this;
};

void ShiftRegisterPWM::set(uint8_t pin, uint8_t value) 
{
  value = (uint8_t) (value / 256.0 * resolution + .5); // round
  uint8_t shiftRegister = pin / 8;
  for (int t = 0; t < resolution; ++t)
  {
    // set (pin % 8)th bit to (value > t)
    this->data[t + shiftRegister * resolution] ^= (-(value > t) ^ this->data[t + shiftRegister * resolution]) & (1 << (pin % 8));
  }
};

void ShiftRegisterPWM::update()
{
  for (int i = this->shiftRegisterCount - 1; i >= 0; i--)
  {
    this->shiftOut(this->data[time + i * resolution]);
  }
  ShiftRegisterPWM_toggleLatchPin();
  
  if (++this->time == resolution) {
    this->time = 0;
  }
};

void ShiftRegisterPWM::interrupt() const
{
  this->interrupt(ShiftRegisterPWM::UpdateFrequency::Medium);
};

void ShiftRegisterPWM::interrupt(UpdateFrequency updateFrequency) const
{
  cli(); // disable interrupts
  
  // reset
  TCCR1A = 0; // set TCCR1A register to 0
  TCCR1B = 0; // set TCCR1B register to 0
  TCNT1  = 0; // reset counter value

  switch (updateFrequency) {
    case VerySlow: // exactly 6,400 Hz interrupt frequency
      OCR1A = 2499; // compare match register
      TCCR1B |= (1 << CS10); // prescaler 1
      break;
      
    case Slow: // exactly 12,800 Hz interrupt frequency
      OCR1A = 1249; // compare match register
      TCCR1B |= (1 << CS10); // prescaler 1
      break;
      
    case Fast: // aprox. 35,714 Hz interrupt frequency
      OCR1A = 55; // compare match register
      TCCR1B |= (1 << CS11); // prescaler 8
      break;
      
    case SuperFast: // approx. 51,281.5 Hz interrupt frequency
      OCR1A = 311; // compare match register
      TCCR1B |= (1 << CS10); // prescaler 1
      break;
      
    case Medium: // exactly 25,600 Hz interrupt frequency
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
