TinyGPS::TinyGPS() {};

//Repeatedly feed it characters from your
void TinyGPS::setup( ) {
  Serial1.begin(9600);
  Serial.println(F("GPS INITIALISED"));
}

double TinyGPS::getBearing() {
  return bearing;
}

void TinyGPS:: sendNMEA() {
  if ( !complete )
    return;
  //data.sendNMEA( TINY_GPS_ID, TINY_GPS, sentence );
  //complete = false;
  //sentence = "";
}

bool TinyGPS::wait() {
  return complete;
}

bool TinyGPS::readNMEA( char chr ) {
  if ( complete )
    return false;
  sentence += chr;
  if (chr == 0x0A)
    complete = true;
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent1() {
  if ( tinyGPS.wait())
    return;
  //while (Serial1.available()) {
  //  tinyGPS.readNMEA((char)Serial1.read());
  //}
}

void TinyGPS::loop() {
  //Serial.println(F("CHECKING GPS"));
  bool update = false;
  while ( Serial1.available() > 0 ) {
    char chr = Serial1.read();
    if (( millis() > 5000 ) && gps.charsProcessed() < 10 ) {
      vessel.update( latitude, longitude, bearing, speed, update );
      return;
    }
    gps.encode( chr );
    update = gps.location.isUpdated();
    if ( update) {
      Serial.println(F("GPS Location Updated"));
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      bearing = gps.course.deg();
      speed = gps.speed.mps();
      update = true;
      vessel.update( latitude, longitude, bearing, speed, update );
    }
  }

  if ( gps.failedChecksum() ) {
    Serial.print(F("Sentences that failed checksum="));
    Serial.println(gps.failedChecksum());
  }
}
