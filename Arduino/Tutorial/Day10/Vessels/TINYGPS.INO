#include "TinyGPS.h"

#define GPS_LOCATED F("\n\nGPS Location Updated: ")

//Repeatedly feed it characters from your
void TinyGPS::setup( ) {
  Serial1.begin(9600);
  Serial.println(F("GPS INITIALISED"));
}

String TinyGPS::getConfig() {
  return CONFIG_GPS;
}

double TinyGPS::getLatitude() {
  return latitude;
}

double TinyGPS::getLongitude() {
  return longitude;
}

double TinyGPS::getHeading( double latFrom,  double lonFrom, double latTo, double lonTo ) {
  return gps.courseTo( latFrom, lonFrom, latTo, lonTo );
}

double TinyGPS::getDistance( double latFrom, double lonFrom, double latTo, double lonTo ) {
  return gps.distanceBetween( latFrom, lonFrom, latTo, lonTo );
}

void TinyGPS::loop( bool nmea) {
  //Serial.println(F("CHECKING GPS"));
  bool updated = false;
  unsigned long current = millis();
  String str = String();
  while (Serial1.available() && ( millis() < ( current + TIME_OUT  ))) {
    char chr = Serial1.read();
    //readNMEA( chr );
    //Serial.print(chr );
    str += chr;
    gps.encode( chr );
    updated = gps.location.isUpdated();
    if ( updated ) {

      latitude = gps.location.lat();
      longitude = gps.location.lng();
      heading = gps.course.deg();
      speed = gps.speed.mps();
      logger.println( GPS_LOCATED);
      Serial.print(GPS_LOCATED);
      Serial.print( latitude, 6 ); Serial.print("E ");
      Serial.print( longitude, 6 ), Serial.println("N\n\n "); // bearing, speed );
      break;
    }
  }

  if (( latitude >= 0 ) && ( longitude >= 0 ))
    vessel.update( latitude, longitude, voltage.getVoltage(), updated );
  if ( nmea && !( str.length() ==0 )) {
     data.sendNMEA(TINY_GPS_ID, TINY_GPS, str );
  }
  //if ( gps.failedChecksum() ) {
  //  Serial.print(F("Sentences that failed checksum="));
  //  Serial.println(gps.failedChecksum());
  //}
}
