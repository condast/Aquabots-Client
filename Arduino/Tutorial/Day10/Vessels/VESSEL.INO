//Constructor
Vessel::Vessel() {};

void Vessel::setup() {
  Serial.println(F("SETUP VESSEL" ));
  enable = true;//sdcard.readBoolean( CONFIG, AUTONOMY );
  Serial.print(F("VESSEL ENABLED: ")); Serial.println( enable );
  if (!enable )
    return;
  speed = 0;
  range = DEFAULT_RANGE;
  waypointIndex = 0;

  Serial.print(F("MAINTENANCE MODE: ")); Serial.println( maintenance );

  bool init = false;//!sdcard.readBoolean( CONFIG, INITIALISE );
  Serial.print(F("INITIALISING VESSEL: ")); Serial.println( !init );
  while (!init) {
    init = initialise();
  }
  controller.setup();
  Serial.print(F("VESSEL READY ")); Serial.println( init );
  delay( 1000);
}

boolean Vessel::initialise() {
  if (!enable || initialised )
    return true;
  bool ready = controller.initialise();
  return ready;
}

bool Vessel::runMaintenance( unsigned int counter ) {
  if (!enable || !maintenance )
    return true;
  bool ready = controller.isReady();
  int thrust = 0;
  Serial.print(F("MAINTENANCE: ")); Serial.println( ready );
  double heading = 0;
  if ( counter > 0) {
    Serial.print(F("COUNTING: ")); Serial.print( counter );
    if ( counter < 720 ) {
      int cnt_bearing = counter % 720;
      heading = ( cnt_bearing < 360 ) ? cnt_bearing++ : cnt_bearing--;
      heading = ( heading > 360 ) ? 0 : ( heading < -360) ? 0 : heading;
      Serial.print(F(" HEADING ")); Serial.print( heading ); // Serial.print(F(", TURN ")); Serial.println( turn_active );
    } else if ( counter < 820 ) {
      heading = 0;
      Serial.print(F("THRUST INCREASING")); Serial.println( thrust );
      thrust++;
    } else if ( counter < 1020 ) {
      Serial.print(F("THRUST DECREASING")); Serial.println( thrust );
      thrust--;
    } else if ( counter < 1120 ) {
      Serial.print(F("THRUST INCREASING")); Serial.println( thrust );
      thrust++;
    }
    counter %= 1120;
  }
  controller.loop( heading, data.thrust);
  return true;
}

/**
   Send the current location and get a stack with the next steps to take.
   The first latlnh should be entered last, so that the can be popped
*/
bool Vessel::update( double latitde, double longitde, double voltage, bool updated ) {
  //Serial.println(F("UPDATED: "));
  if ( ! webClient.connect() )
    return false;
  webClient.setContext( AQUABOTS_VESSEL_CONTEXT );
  data.latitude = latitde;
  data.longitude = longitde;
  String url = F("&lo=");
  url += String( longitde, 20 );
  url += F("&la=");
  url += String( latitde, 20);
  url += F("&h=");
  url += heading;
  url += F("&s=");
  url += speed;
  url += F("&vt=");
  url += voltage;
  url += F("&u=");
  url += updated;
  //Serial.print(F("UPDATE VESSEL: "));
  //Serial.println( url);

  boolean result = webClient.sendHttp( WebClient::UPDATE, false, url);
  if (!result ) {
    webClient.disconnect();
    if ( current.complete ) {
      if ( current.index < sizeof( waypoints ) - 1) {
        int index = current.index+1;
        current = waypoints[ index];
      }
    }
    return false;
  }

  //String response = webClient.printResponse( WebClient::UPDATE );
  //Serial.print(F("\n\nHEADING: \n")); Serial.println( response);
  size_t capacity = JSON_OBJECT_SIZE(15) + 80;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, webClient.client);
  if (error) {
    Serial.print(F("Parsing update failed!")); Serial.println( error.c_str() );
    doc.clear();
    webClient.disconnect();
    return false;
  }
  JsonObject root = doc.as<JsonObject>();
  data.bearing = root["b"];
  data.thrust = root["t"];
  data.time = root["tm"];
  data.active = root["a"];
  //Serial.print(F("DATA: ")); Serial.println(  data.active );
  data.manual = root["mn"];
  doc.clear();
  webClient.disconnect();
  return true;
}

/**
   Get the waypoint for this desired index location. The server may override this
   with a more pressing one (e.g after a clear)
*/
bool Vessel::getWaypoint() {
  if (!enable )
    return false;
  String url = F("&index=");
  url += String( waypointIndex++ );
  waypointIndex %= MAX_WAYPOINTS;
  if ( ! webClient.connect()) {
    return false;
  }
  webClient.setContext( AQUABOTS_VESSEL_CONTEXT );
  Serial.print(F("GETTING WAYPOINTS: "));
  boolean result = webClient.sendHttp( WebClient::WAYPOINT, false, url );
  if ( !result ) {
    webClient.disconnect();
    return result;
  }
  size_t capacity = JSON_OBJECT_SIZE(7);
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, webClient.client);
  if (error) {
    Serial.print(F("Parsing Waypoint failed!")); Serial.println( error.c_str() );
    doc.clear();
    webClient.disconnect();
    return false;
  }
  JsonObject root = doc.as<JsonObject>();
  if ( root.size() < 1 ) {
    webClient.disconnect();
    stop();
    doc.clear();
    return false;
  }
  int index = root[F("i")];
  if ( index > sizeof( waypoints ))
    return false;
  char* chr_arr = root[F("nm")];
  //sdcard.writeToIndex( WAYPOINTS_FILE, index, buffer );
  current.index = index;
  current.name = String( chr_arr );
  current.latitude = root[F("la")];
  current.longitude = root[F("lo")];
  current.complete = root["cp"];
  current.last = root["last"];
  waypoints[index] = current;
  webClient.disconnect();
  doc.clear();
  Serial.print(F("WAYPOINTS RECEIVED: ")); Serial.println(index);
  return true;
}

/**
   Send the current location and get a stack with the next steps to take.
   The first latlnh should be entered last, so that the can be popped
*/
bool Vessel::updateWaypoints( int index ) {
  Serial.print(F("UPDATE WAYPOINTS: "));
  webClient.setContext( AQUABOTS_VESSEL_CONTEXT );
  if ( ! webClient.connect()) {
    //webClient.disconnect();
    return false;
  }
  boolean result = false;
  if ( webClient.client.connected() ) {
    result = webClient.sendHttp( WebClient::WAYPOINTS, false, String( index ));
    if (!result ) {
      webClient.disconnect();
      return result;
    }

    size_t capacity = JSON_OBJECT_SIZE(6) + 100;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, webClient.client);
    if (error) {
      Serial.print(F("Parsing update waypoints failed!")); Serial.println( error.c_str() );
      doc.clear();
      webClient.disconnect();
      return false;
    }
    JsonObject root = doc.as<JsonObject>();
    int i = root["i"];
    char* chr_arr = root[F("nm")];
    waypoints[i].name = String( chr_arr);
    waypoints[i].latitude = root[F("la")];
    waypoints[i].longitude = root[F("lo")];
    waypoints[i].complete = root[F("cp")];
    waypoints[i].last = root[F("last")];
    doc.clear();
    webClient.disconnect();
    Serial.print(F("WAYPOINTS RECEIVED: ")); Serial.println(index);
    result = true;
  }
  return result;
}

//Is used in the loop function
void Vessel::stop() {
  //Serial.println("\n\nSTOPPING!");
  controller.stop();
  speed = 0;
}

void Vessel::loop( double headng) {
  heading = headng;
  if (!enable || !data.active ) {
    stop();
    return;
  }
  //logger.println( "Course: " + heading );
  double diff = (int)(heading - data.bearing);
  double hd = ( data.manual ) ? data.bearing : diff;
  //Serial.print(F("VESSEL: (")); Serial.print( heading ); Serial.print( " = "); Serial.print( hd ); Serial.print( ", "); Serial.print( data.thrust ); Serial.println( F(")"));
  controller.loop( hd, data.thrust );
}
