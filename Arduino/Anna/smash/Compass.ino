Compass::Compass() {
  start = false;
  amount = 1;
  counter = 0;
  bearing = 0;
};

//Repeatedly feed it characters from your
void Compass::setup( ) {
  Serial2.begin(4800);
  Serial.println("Polling Compass");
}

float Compass::getBearing() {
  return bearing;
}

void Compass::setFilter( int amnt ) {
  counter = 0;
  amount = (amnt <= 0) ? 1 : amnt;
}

float Compass::getFiltered() {
  return filtered;
}

void Compass::loop( bool enabled ) {
  if (!enabled )
    return;
  if (Serial2.available()) {
    char chr =  Serial2.read();
    if ( chr == '$') {
      //Serial.print(F("READING: "));
      start =  true;
      nmea = "";
    } else if ( chr == '\n' ) {
      start = false;
      if ( !nmea.startsWith( NMEA_COMPASS ))
        return;
      int bg = nmea.indexOf(',');
      int nd = nmea.indexOf(',', bg + 1);
      bearing = nmea.substring(bg + 1, nd).toFloat();
      if ( counter < amount )
        counter++;
      filtered = (filtered * (counter - 1) + bearing ) / counter;
      Serial.print(F(" = "));
      Serial.print( bearing);
      Serial.print(F(" ("));
      Serial.print( filtered );
      Serial.println(F(" ) DONE: "));
    }
    if ( start ) {
      //Serial.print( chr );
      nmea += chr;
    }
  }
  if ( nmea.length() != 0 ) {
    data.sendNMEA(NASA_COMPASS_ID, NASA_COMPASS, nmea );
  }
  //Serial.println( Compass );
}
