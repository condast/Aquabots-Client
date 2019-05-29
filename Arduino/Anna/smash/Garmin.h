#ifndef Garmin_h
#define Garmin_h

#include "Arduino.h"
#include "Vessel.h"

#define GARMIN_ID "bathymetry.garmin.ds70"
#define GARMIN "GARMIN"

class Garmin {

  public: Garmin(void);
    void setup();
    void loop( );

    private: bool enabled;
};

#endif


