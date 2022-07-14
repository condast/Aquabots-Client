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

  String url = F("&latitude=");
  url += String( latitude);
  url += F("&longitude=");
  url += String( longitude);

  boolean result = webClient.sendHttp( WebClient::REGISTER_VESSEL, false, url);
  FieldData field;
  if (!result ) {
    webClient.disconnect();
    return result;
  }
  if ( webClient.client.connected() ) {
    size_t capacity = JSON_OBJECT_SIZE(5) + 100;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, webClient.client);
    if (error) {
      Serial.println(F("Parsing field failed!"));
      webClient.disconnect();
      return false;
    }
    JsonObject root = doc.as<JsonObject>();

    String str = root[F("name")];
    field.name =  str;
    field.latitude = root[F("latitude")];
    field.longitude = root[F("longitude")];
    Serial.print(F("Field retrieved: ")); Serial.println( str );
    webClient.disconnect();
    initialised = true;
    return true;
  }
  webClient.disconnect();
  return false;
}

void Field::loop( double latitude, double longitude ) {
  Serial.println(F("Field: updating"));
  bool result = requestField( latitude, longitude);
  if (result) {
    Serial.print(F("FIELD found: ")); Serial.println( field.name );
  }
}
