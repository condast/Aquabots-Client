#ifndef Logger_h
#define Logger_h

#include "Arduino.h"

class Logger {

  private: boolean logger = false;

  public: Logger(void);
    boolean setup();
    void setLogger( boolean choice );
    void print( String msg );
    void println( String msg );
    boolean requestLog();
    boolean logMessage( String message );
    };

#endif


