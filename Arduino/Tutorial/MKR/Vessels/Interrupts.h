#ifndef Interrupts_h
#define Interrupts_h

// These define's must be placed at the beginning before #include "SAMDTimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

#define SECONDS 10 //0.1 sec
#define TEN_SEC 40
#define MINUTES 600

class Interrupts {

  private:
    // Init SAMD timer TIMER_TC3
    //Create a sempahore
    bool lock;
    void ISR_Routine();

  public: Interrupts(void);
    int int_counter;
    bool flank; //10Hz
    bool sec_flank;//seconds
    bool min_flank;//minutes
    bool tensec_flank;//10 sec.

    void setup();
    void clear();
    bool getLock();
    void setLock( bool lock );
    bool getFlank();
    bool getSecondsFlank();
    void clearSecondsFlank();
    void clearFlank();
    int getCounter();
};
#endif
