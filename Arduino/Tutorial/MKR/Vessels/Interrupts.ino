#define TENTH_SECONDS 1
#define SECONDS 10


Interrupts::Interrupts() {};

/**
    Set timer0 to increment with steps of 10 Hz
*/
void Interrupts::setup() {
  lock = false;
  flank = false;
  sec_flank = false;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |                 // Enable GCLK0 for TC4 and TC5
                      GCLK_CLKCTRL_GEN_GCLK0 |             // Select GCLK0 at 48MHz
                      GCLK_CLKCTRL_ID_TC4_TC5;             // Feed GCLK0 output to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY);                       // Wait for synchronization

  TC4->COUNT16.CC[0].reg = 29999;                          // Set the TC4 CC0 register as the TOP value in match frequency mode
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);                // Wait for synchronization

  NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  TC4->COUNT16.INTENSET.reg = TC_INTENSET_OVF;             // Enable TC4 overflow (OVF) interrupts

  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCSYNC_PRESC |     // Reset timer on the next prescaler clock
                            TC_CTRLA_PRESCALER_DIV8 |      // Set prescaler to 8, 48MHz/8 = 6MHz
                            TC_CTRLA_WAVEGEN_MFRQ |        // Put the timer TC4 into match frequency (MFRQ) mode
                            TC_CTRLA_MODE_COUNT16;         // Set the timer to 16-bit mode
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);                // Wait for synchronization

  TC4->COUNT16.CTRLA.bit.ENABLE = 1;                       // Enable the TC4 timer
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);
}

// In SAMD, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void Interrupts::ISR_Routine()
{
  interrupt.flank = true;
  interrupt.int_counter++;
  interrupt.int_counter %= SECONDS;
  if ( interrupt.int_counter > 0 )
    return;
  interrupt.sec_flank = true;
  vessel.loop( compassModule.getHeading());

}

boolean Interrupts::getLock() {
  return lock;
}

void Interrupts::setLock( boolean lck ) {
  lock = lck;
}

int Interrupts::getCounter() {
  return int_counter;
}

boolean Interrupts::getFlank() {
  return flank;
}

boolean Interrupts::getSecondsFlank() {
  return sec_flank;
}

void Interrupts::clearSecondsFlank() {
  sec_flank = false;
}

void Interrupts::clearFlank() {
  flank = false;
}
