ServoController::ServoController() {};

void ServoController::setup( ) {
  //compass.setup();
  //compass.setFilter( 10 );
  offset =  -5;
  clipRange = SERVO_RANGE;
  offset = 0;//sdcard.read( CONFIG, CLIP_RANGE ).toInt();
  bearingServo.attach( PWM_OUT_BEARING );
  bearingServo.write( 90 );
  bearing = 0;
  heading = 0;
  enable = true;
}

boolean ServoController::initialise() {
  ready = true;
  return ready;
}

bool ServoController::maintenance() {
  int counter = 0;
  int br = 0;
  do {
    Serial.print("Moving to: "); Serial.println( br );
    do {
      counter++;
      counter %= 100;
      int speed = -100 + 2 * counter;
      setBearing( br );
      loop( br);
      delay(100);
    } while ( counter != 0 );
    br -= 10;
    br = ( 360 + br ) % 360;
  } while (true);
}

/**
   Used by interrupt handler
*/
void ServoController::toggle( ) {
  /* Default nothing */
}

bool ServoController::isReady() {
  return ready;
}

/**
   get the angle for the servo, based on the desired bearing
*/
void ServoController::setBearing( double brng ) {
  bearing = brng;
  Serial.print(F("Bearing: ")); Serial.print( bearing );  Serial.print(F(" Heading: ")); Serial.println( heading );
}

/**
   get the angle for the servo, based on the desired bearing
*/
void ServoController::stop( ) {
    bearingServo.write( 90 );
    bearing = 0;
    heading = 0;
}

void ServoController::loop( double brng) {
  setBearing( brng );
  //compass.loop();
  //if ( compass.isEnabled() ) {
  //  heading = compass.getFiltered();
  //} else if ( tendof.isEnabled() ) {
  //  heading = tendof.getHeading();
  //} else {
    heading = bearing;
  //}
  double diff = bearing - heading;
  Serial.print(F("\n\nCorrecting: ")); Serial.println( diff + offset);
  int newBearing = map(( offset + diff ), 0, 360, clipRange, 180 - clipRange );
  bearingServo.write( newBearing );
}
