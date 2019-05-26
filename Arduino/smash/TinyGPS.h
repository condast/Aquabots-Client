#ifndef TinyGPS_h
#define TinyGPS_h

#include "Arduino.h"
#include "TinyGPS++.h"

#define TINY_GPS "TinyGPS"
#define TINY_GPS_ID "gps.tiny"

class TinyGPS {

  private:
    bool complete; //general purpose flag for reading nmea sentences
    String sentence; 
    double bearing;
    double speed;
    double latitude;
    double longitude;
    TinyGPSPlus gps;

  public: TinyGPS(void);
    void setup();
    double getBearing();
    bool wait();//wait for processing of the nmea sentence
    void sendNMEA();
    bool readNMEA( char chr); //used by SerialEvent
    void loop();
};

#endif


