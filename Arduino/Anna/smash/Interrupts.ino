int int_counter;
int min_counter;

//Activate interrupt Timer2
ISR(TIMER2_COMPA_vect) {
  vessel.interrupt();
  interrupt.flank = true;
  if ( interrupt.getLock() )
    return;
  int_counter++;
  int_counter %= SECONDS;
  if ( int_counter == 0 )
    interrupt.sec_flank = true;
  else
    return;
    
  interrupt.min_flank = false;
  interrupt.tensec_flank = false;
  min_counter++;
  min_counter %= MINUTES;
  if ( min_counter == 0)
    interrupt.min_flank = true;
  if (( min_counter % TEN_SEC ) == 0)
    interrupt.tensec_flank = true;
}

Interrupts::Interrupts() {};

/**
   Set timer0 to increment with steps of 10 Hz
*/
void Interrupts::setup() {
  cli();//stop interrupts

  int_counter = 0;
  min_counter = 0;
  lock = false;

  //set timer2 interrupt at 2kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 10 Hz increments
  OCR2A = 156;// = (16*10^6) / (10*102) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS00 and CS02 bits for 1024 prescaler
  TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  clear();
  sei();
}

void Interrupts::clear() {
  setLock( true );
  sec_flank = false;//0.1 seconds
  min_flank = false;//minutes
  tensec_flank = false;//10 sec.
  setLock( false );
}

void Interrupts::clearFlank() {
  flank = false;
}
bool Interrupts::getLock() {
  return lock;
}

void Interrupts::setLock( bool lck ) {
  lock = lck;
}

/**
 * Is set every 10 Hz
 */
bool Interrupts::getFlank() {
  return flank;
}

bool Interrupts::getSecondsFlank() {
  return sec_flank;
}

bool Interrupts::getMinutesFlank() {
  return min_flank;
}

bool Interrupts::getTenSecondsFlank() {
  return tensec_flank;
}
