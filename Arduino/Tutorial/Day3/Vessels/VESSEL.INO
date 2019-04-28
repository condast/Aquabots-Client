//Constructor
Vessel::Vessel() {};

void Vessel::setup() {
  Serial.println(F("SETUP VESSEL" ));
  enable = true;
  Serial.print(F("VESSEL ENABLED: ")); Serial.println( enable );
  if (!enable )
    return;
  speed = 0;
  range = DEFAULT_RANGE;

  Serial.print(F("VESSEL READY "));
  delay( 1000);
}

/**
   Send the current location and get a stack with the next steps to take.
   The first latlnh should be entered last, so that the can be popped
*/
bool Vessel::update( double latitde, double longitde, double bearing, double speed, bool updated ) {
  if ( ! webClient.connect() )
    return false;
  webClient.setContext( AQUABOTS_VESSEL_CONTEXT );
  data.latitude = latitde;
  data.longitude = longitde;
  String url = F("&lo=");
  url += String( data.longitude, 8 );
  url += F("&la=");
  url += String( data.latitude, 8);
  url += F("&b=");
  url += bearing;
  url += F("&s=");
  url += speed;
  url += F("&u=");
  url += updated;
  //Serial.print(F("UPDATE VESSEL: "));
  //Serial.println( url);

  boolean result = webClient.sendHttp( WebClient::UPDATE, false, url);
  if (!result ) {
    webClient.disconnect();
    return false;
  }

  //String response = webClient.printResponse( WebClient::UPDATE );
  //Serial.print(F("\n\nHEADING: \n")); Serial.println( response);

  size_t capacity = JSON_OBJECT_SIZE(11) + 79;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, webClient.client);
  if (error) {
    Serial.println(F("Parsing update failed!"));
    webClient.disconnect();
    return false;
  }
  JsonObject root = doc.as<JsonObject>();
  data.heading = root["h"];
  data.thrust = root["t"];
  data.time = root["tm"];
  data.manual = root["mn"];
  webClient.disconnect();
  setCourse( data.heading, data.thrust);
  return true;
}

void Vessel::setCourse( double heading, double thrust ) {
}

void Vessel::stop() {
  Serial.println("\n\nSTOPPING!!!");
  bearing = 0;
  speed = 0;
}

void Vessel::loop( double heading) {
  if ( !enable )
    return;
}
