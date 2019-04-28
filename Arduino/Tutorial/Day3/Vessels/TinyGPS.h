#ifndef TinyGPS_h
#define TinyGPS_h

#define TINY_GPS "TinyGPS"
#define TINY_GPS_ID "gps.tiny"

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

class TinyGPS{

  private:
    TinyGPSPlus gps;
    double latitude;
    double longitude;
    double bearing;
    double speed;

  public: TinyGPS(void);
    double getLatitude();
    double getLongitude();
    double getBearing( double latFrom,  double lonFrom, double latTo, double lonTo );
    double getDistance( double latFrom, double lonFrom, double latTo, double lonTo );
    void setup();
    bool wait();//wait for processing of the nmea sentence
    void loop();
};

#endif
