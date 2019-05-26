//Constructor
Vessel::Vessel() {};

void Vessel::setup() {
  Serial.println(F("SETUP VESSEL" ));
  enable = true;//sdcard.readBoolean( CONFIG, AUTONOMY );
  Serial.print(F("VESSEL ENABLED: ")); Serial.println( enable );
  if (!enable )
    return;
  latitude = 0;
  longitude = 0;
  counter = 0;
  thrust = 0;
  speed = 0;
  bearing = 0;
  turn_active = sdcard.read( CONFIG, TURN_ACTIVE ).toInt();
  turn_speed = sdcard.read( CONFIG, TURN_SPEED ).toInt();
  turn_angle = sdcard.read( CONFIG, TURN_ANGLE ).toInt();
  turn = sdcard.read( CONFIG, TURN ).toInt();
  if ( turn < turn_active )
    turn = turn_active;
  turn_offset = sdcard.read( CONFIG, TURN_OFFSET ).toInt();
  if ( turn_offset == 0 )
    turn_offset = -5;
  speed_offset = sdcard.read( CONFIG, SPEED_OFFSET ).toInt();
  if ( speed_offset == 0 )
    speed_offset = -4;
  bearingServo.attach( PWM_OUT_BEARING );
  speedServo.attach( PWM_OUT_SPEED );

  maintenance = sdcard.readBoolean( CONFIG, MAINTENANCE );
  Serial.print(F("MAINTENANCE MODE: ")); Serial.println( maintenance );

  bool init = !sdcard.readBoolean( CONFIG, INITIALISE );
  Serial.print(F("INITIALISING VESSEL: ")); Serial.println( !init );
  while (!init) {
    init = initialise();
  }
  Serial.print(F("VESSEL READY ")); Serial.println( init );
  int counter = 0;
  delay( 1000);
  Serial.print( " Offset: "); Serial.println( encoder.getOffset());

  turn_speed = 10;
  turn_angle = 0;
  brng = 0;
}

boolean Vessel::initialise() {
  if (!enable || initialised )
    return true;
  bool ready = encoder.isReady();
  Serial.print(F("INITIALISING... "));
  setTurn( 25 );
  ready = false;
  while ( !ready ) {
    ready = encoder.initOffset();
  }
  setTurn( 0 );
  Serial.print( "INITIALSED, offset is: "); Serial.println( encoder.getOffset() );
  return true;
}

bool Vessel::runMaintenance( int counter ) {
  if (!enable || !maintenance )
    return true;
  bool ready = encoder.isReady();
  counter = 0;
  int br = 0;
  do {
    Serial.print("Moving to: "); Serial.println( br );
    int fix = 0;
    do {
      counter++;
      counter %= 100;
      int speed = -100 + 2 * counter;
      fix = setCourse( br, speed );
      delay(100);
    } while ( counter != 0 );
    br += 10;
    br = ( 360 + br ) % 360;
  } while (true);
}

/**
   Help function to calculate the ideal speed
*/
int Vessel::getTurnSpeed( int diff ) {
  int abs_diff = ( diff < 0) ? -diff : diff;
  if (( abs_diff > 30 ) || (( diff > -turn_angle ) && ( diff < turn_angle )))
    return diff;
  int speed = turn_speed + abs_diff;
  return ( diff < -turn_angle ) ? -speed : speed;
}

int Vessel::setCourse( double newbearing, double thrust ) {
  if ( !enable )
    return;
  encoder.loop();
  //Serial.print(F("SETTING COURSE: ")); Serial.print( newbearing ); Serial.print( ", "); Serial.print( thrust );
  double diff = encoder.getDifferenceInPercent( newbearing );
  int turnSpeed = getTurnSpeed( diff );
  //Serial.print(F("Check angle for  ")); Serial.print( encoder.getEncoder()); Serial.print( F( " diff = " ));
  //Serial.print( diff ); Serial.print(F(": Turn Speed ")); Serial.println( turnSpeed );
  setTurn( turnSpeed );
  setThrust( thrust );
  //Serial.print(F(" diff: : ")); Serial.println( diff );
  return diff;
}

/**
   Set the turn speed of the motor
*/
Vessel::setTurn( double turnSpeed ) {
  int newTurn = map( turnSpeed, -100, 100, MIN_SERVO, MAX_SERVO );
  //Serial.print(F("Turn Speed ")); Serial.print( turnSpeed ); Serial.print(F(" PWM ")); Serial.println( turn_offset + newTurn );
  if ( newTurn != bearingServo.read() )
    bearingServo.write( turn_offset + newTurn );
}

Vessel::setThrust( double thrust ) {
  int newThrust = map( thrust, -100, 100, MIN_SERVO, MAX_SERVO );
  if ( newThrust != speedServo.read() )
    speedServo.write( speed_offset + newThrust );
}

/**
   Get the waypoints for this vessel
*/
bool Vessel::getWaypoint( ) {
  if (!enable )
    return;
  boolean result = webClient.getWaypoint();
  if ( !result ) {
    return result;
  }
  size_t capacity = JSON_OBJECT_SIZE(5) + 40;
  DynamicJsonBuffer jsonBuffer(capacity);

  // Parse JSON object
  String buffer = webClient.printResponse( WebClient::WAYPOINTS );
  JsonObject& root = jsonBuffer.parseObject( buffer );
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
    jsonBuffer.clear();
    return;
  }

  if ( root.size() < 1 )
    return false;
  int index = root["i"];
  sdcard.writeToIndex( WAYPOINTS_FILE, index, buffer );

  //Serial.print( "WAYPOINTS: " );
  //Serial.println( str );

  Serial.print(F("WAYPOINTS FOUND: "));
  return true;
}

/**
   Send the current location and get a stack with the next steps to take.
   The first latlnh should be entered last, so that the can be popped
*/
bool Vessel::update( double latitde, double longitde, double bearing, double speed, bool updated ) {
  Serial.println("Updating vessel");
  if ( ! webClient.connect() )
    return;

  latitude = latitde;
  longitude = longitde;
  String url = F("&lo=");
  url += longitude;
  url += F("&la=");
  url += latitude;
  url += F("&b=");
  url += bearing;
  url += F("&s=");
  url += speed;
  url += F("&u=");
  url += updated;
  //Serial.print(F("UPDATE VESSEL: "));
  //Serial.println( url);

  bool result = webClient.sendHttp( WebClient::UPDATE, false, url);
  //Serial.print(F("VESSEL Updated: ")); Serial.println( result );
  if (!result ) {
    Serial.print(F("VESSEL Was NOT Updated: "));
    webClient.disconnect();
    return false;
  }

  size_t capacity = JSON_OBJECT_SIZE(9) + 100;
  DynamicJsonBuffer jsonBuffer(capacity);

  JsonObject& root = jsonBuffer.parseObject( webClient.client );
  if (!root.success()) {
    Serial.println(F("Parsing update failed!"));
    jsonBuffer.clear();
    webClient.disconnect();
    return false;
  }
  data.bearing = root["b"];
  data.thrust = root["t"];
  data.time = root["tm"];
  jsonBuffer.clear();
  bearing = RAD_TO_DEG * data.bearing;
  brng = bearing;
  thrust = data.thrust;
  webClient.disconnect();
  //Serial.print( "Vessel updated, bearing "); Serial.print( bearing); Serial.print( " thrust "); Serial.println( thrust );
  return true;
}

/**
   Send the current location and get a stack with the next steps to take.
   The first latlnh should be entered last, so that the can be popped
*/
bool Vessel::updateWaypoints( ) {
  Serial.print(F("UPDATE WAYPOINTS: "));

  if ( ! webClient.connect())
    return false;
  boolean result = false;
  if ( webClient.client.connected() ) {
    result = webClient.sendHttp( WebClient::WAYPOINTS, false, "");
    if (!result ) {
      webClient.disconnect();
      return result;
    }

    size_t capacity = JSON_OBJECT_SIZE(9) + 100;
    DynamicJsonBuffer jsonBuffer(capacity);

    JsonObject& root = jsonBuffer.parseObject( webClient.client );
    if (!root.success()) {
      Serial.println(F("Parsing update failed!"));
      jsonBuffer.clear();
      webClient.disconnect();
      return false;
    }
    data.bearing = root["b"];
    data.thrust = root["s"];
    data.time = root["t"];
    jsonBuffer.clear();
    webClient.disconnect();
    result = true;
  }
  //setCourse( data.bearing, data.thrust);
  return result;
}

void Vessel::interrupt() {
  if ( !enable )
    return;

  if ( maintenance ) {
    setCourse( 0, -5 );
    //runMaintenance( counter );
    return;
  }

   //bearing = brng;
  //Serial.print( "Running vessel, bearing "); Serial.print( brng); Serial.print( " thrust "); Serial.println( thrust );
  setCourse( brng, thrust );
  //getWaypoint();
}
void Vessel::loop() {
  //synchronize();
  if ( !enable )
    return;
  counter++;
  counter %= 200;

  if ( maintenance ) {
    setCourse( 0, -5 );
    //runMaintenance( counter );
    return;
  }

/*
  bearing = brng;
  if (( counter % 100 ) == 0 ) {
    bearing -= 10;
    bearing = (int)( 360 + bearing )%360;
  }
  thrust = -100 + counter;
  brng = bearing;
  */
  //Serial.print( "Running vessel, bearing "); Serial.print( brng); Serial.print( " thrust "); Serial.println( thrust );
  //getWaypoint();
}
