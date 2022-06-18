#include "VesselGPS.h"

#define GPS_LOCATED F("\n\nGPS Location Updated: ")

//Repeatedly feed it characters from your
void VesselGPS::setup( ) {
  Serial1.begin(9600);
  Serial.println(F("GPS INITIALISED"));
}

String VesselGPS::getConfig() {
  return CONFIG_GPS;
}

double VesselGPS::getLatitude() {
  return latitude;
}

double VesselGPS::getLongitude() {
  return longitude;
}

double VesselGPS::getHeading() {
  return heading;
}

double VesselGPS::getHeading( double latFrom,  double lonFrom, double latTo, double lonTo ) {
  return gps.courseTo( latFrom, lonFrom, latTo, lonTo );
}

double VesselGPS::getDistance( double latFrom, double lonFrom, double latTo, double lonTo ) {
  return gps.distanceBetween( latFrom, lonFrom, latTo, lonTo );
}

void VesselGPS::loop( bool nmea) {
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
    vessel.update( latitude, longitude, voltage.getVoltagePercent(), updated );
  if ( nmea && !( str.length() ==0 )) {
     data.sendNMEA(TINY_GPS_ID, TINY_GPS, str );
  }
  //if ( gps.failedChecksum() ) {
  //  Serial.print(F("Sentences that failed checksum="));
  //  Serial.println(gps.failedChecksum());
  //}
}
