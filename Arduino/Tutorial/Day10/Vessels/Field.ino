Field::Field() {};

void Field::setup() {
  Serial.print(F("SETUP FIELD..."));
  initialised = false;
  delay( 1000 );
  Serial.println(F("done"));
}

/**
   Request the field object
*/
bool Field::requestField( double latitude, double longitude) {
  webClient.setContext( AQUABOTS_VESSEL_CONTEXT );
  if ( !webClient.connect()) {
    return false;
  }

  String url = webClient.createURL( latitude, longitude );
  //Serial.println( url );
  boolean result = webClient.sendHttp( WebClient::FIELD, false, url);
  FieldData field;
  if (!result ) {
    webClient.disconnect();
    return result;
  }
  if ( webClient.client.connected() ) {
    size_t capacity = JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(6) + 112;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, webClient.client);
    if (error) {
      Serial.println(F("Parsing field failed!"));
      webClient.disconnect();
      return false;
    }
    JsonObject root = doc.as<JsonObject>();
    String str = root[F("name")];
    field.name = str;
    field.latitude = root[F("latitude")];
    field.longitude = root[F("longitude")];
    field.length = doc[F("length")]; // 100
    field.width = doc[F("width")]; // 100
    field.angle = doc[F("angle")];    
    Serial.print(F("Field retrieved: ")); Serial.println( field.name );
    webClient.disconnect();
    initialised = true;
    return true;
  }
  webClient.disconnect();
  return false;
}

void Field::loop( double latitude, double longitude ) {
  bool result = requestField( latitude, longitude);
}
