#include "TinyGPS.h"

//Repeatedly feed it characters from your
void TinyGPS::setup( ) {
  Serial1.begin(9600);
  Serial.println(F("GPS INITIALISED"));
}

double TinyGPS::getLatitude() {
  return latitude;
}

double TinyGPS::getLongitude() {
  return longitude;
}

double TinyGPS::getBearing( double latFrom,  double lonFrom, double latTo, double lonTo ) {
  return gps.courseTo( latFrom, lonFrom, latTo, lonTo );
}

double TinyGPS::getDistance( double latFrom, double lonFrom, double latTo, double lonTo ) {
  return gps.distanceBetween( latFrom, lonFrom, latTo, lonTo );
}

void TinyGPS::loop( bool nmea) {
  Serial.println(F("CHECKING GPS"));
  bool updated = false;
  unsigned long current = millis();
  String str = String();
  while (Serial1.available() && ( millis() < ( current + TIME_OUT  ))) {
    char chr = Serial1.read();
    //readNMEA( chr );
    Serial.print(chr );
    str += chr;
    gps.encode( chr );
    updated = gps.location.isUpdated();
    if ( updated ) {

      latitude = gps.location.lat();
      longitude = gps.location.lng();
      bearing = gps.course.deg();
      speed = gps.speed.mps();
      Serial.print(F("\n\nGPS Location Updated: "));
      Serial.print( latitude, 6 ); Serial.print("E ");
      Serial.print( longitude, 6 ), Serial.println("N\n\n "); // bearing, speed );
      break;
    }
  }

  if (( latitude >= 0 ) && ( longitude >= 0 ))
    vessel.update( latitude, longitude, bearing, speed, updated );
  if ( nmea && !( str.length() ==0 )) {
     data.sendNMEA(TINY_GPS, TINY_GPS_ID, str );
  }
  //if ( gps.failedChecksum() ) {
  //  Serial.print(F("Sentences that failed checksum="));
  //  Serial.println(gps.failedChecksum());
  //}
}
