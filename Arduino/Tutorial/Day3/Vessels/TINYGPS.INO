#include "TinyGPS.h"

TinyGPS::TinyGPS(){};

//Repeatedly feed it characters from your
void TinyGPS::setup( ) {
  Serial1.begin(9600);
  Serial.println(F("GPS INITIALISED"));
}

double TinyGPS::getBearing( double latFrom,  double lonFrom, double latTo, double lonTo ) {
  return gps.courseTo( latFrom, lonFrom, latTo, lonTo );
}

double TinyGPS::getDistance( double latFrom, double lonFrom, double latTo, double lonTo ) {
  return gps.distanceBetween( latFrom, lonFrom, latTo, lonTo );
}

double TinyGPS::getLatitude() {
  return latitude;
}

double TinyGPS::getLongitude() {
  return longitude;
}

void TinyGPS::loop() {
  Serial.println(F("CHECKING GPS"));
  bool updated = false;
  unsigned long current = millis();

 while (Serial1.available() && ( millis() < ( current + TIME_OUT  ))) {
    char chr = Serial1.read();
    //readNMEA( chr );
    Serial.print(chr );
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
    //Serial.println();
    //sendNMEA();

  }

  if (( latitude >= 0 ) && ( longitude >= 0 ))
    vessel.update( latitude, longitude, bearing, speed, updated );
  //if ( gps.failedChecksum() ) {
  //  Serial.print(F("Sentences that failed checksum="));
  //  Serial.println(gps.failedChecksum());
  //}
}
