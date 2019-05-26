#ifndef Options_h
#define Options_h

#include "Arduino.h"
#include <avr/pgmspace.h>

#define OPTIONS_STORE 1

class Options {

  public: Options(void);

    void setup();
    boolean isLogging();
    boolean isDebugging();
    boolean isAutonomous();
    boolean hasVisual();
    boolean hasBathymetry();
    void getOptions( int request, String data );
    void getOptions();

  private: int options;
};

#endif

