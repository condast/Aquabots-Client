#ifndef TinyGPS_h
#define TinyGPS_h

#define TINY_GPS "TinyGPS"
#define TINY_GPS_ID "gps.tiny"

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

class TinyGPS {

  private:
    bool complete; //general purpose flag for reading nmea sentences
    String sentence;
    double bearing;
    double speed;
    double latitude;
    double longitude;
    TinyGPSPlus gps;
    //void sendNMEA();
    //bool readNMEA( char chr); //used by SerialEvent

  public: TinyGPS(void);
    void setup();
    double getBearing();
    double getLatitude();
    double getLongitude();
    double getBearing( double latFrom, double lonFrom, double latTo, double lonTo );
    double getDistance( double latFrom, double lonFrom, double latTo, double lonTo );
    bool wait();//wait for processing of the nmea sentence
    void loop();
};

#endif


