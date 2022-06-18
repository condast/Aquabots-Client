#ifndef VesselGPS_h
#define VesselGPS_h

#define TINY_GPS "VesselGPS"
#define TINY_GPS_ID "gps.tiny"

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define CONFIG_GPS F("'id'='org.satr.arnac.gps.tiny', 'type'='seeed.grove.gps', 'unique'='true', 'state'='passed'")

class VesselGPS{

  private:
    TinyGPSPlus gps;
    double latitude;
    double longitude;
    double heading;
    double speed;

  public: VesselGPS(void){};
    void setup();
    String getConfig();
    double getLatitude();
    double getLongitude();
    double getHeading();
    double getHeading( double latFrom, double lonFrom, double latTo, double lonTo );
    double getDistance( double latFrom, double lonFrom, double latTo, double lonTo );
    bool wait();//wait for processing of the nmea sentence
    
    //If NMEA is true, nmea messages are transmitted
    void loop( bool nmea );
};

#endif
