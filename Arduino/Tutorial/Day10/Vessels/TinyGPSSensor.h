#ifndef TinyGPSSensor_h
#define TinyGPSSensor_h

#define TINY_GPS "TinyGPS"
#define TINY_GPS_ID "gps.tiny"

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define CONFIG_GPS F("'id'='org.satr.arnac.gps.tiny', 'type'='seeed.grove.gps', 'unique'='true', 'state'='passed'")

class TinyGPSSensor{

  private:
    TinyGPSPlus gps;
    double latitude;
    double longitude;
    double heading;
    double speed;

  public: TinyGPSSensor(void){};
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
