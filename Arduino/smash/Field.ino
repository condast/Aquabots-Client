Field::Field() {};

void Field::setup() {
  Serial.print(F("SETUP FIELD..."));
  initialised = false;
  do {
    requestField();
    Serial.print(F("."));
  } while ( !initialised );
  delay( 1000 );
  Serial.println(F("done"));
}

/**
   Request the field object
*/
bool Field::requestField() {
  if ( webClient.connect()) {
    bool result = webClient.sendHttp( WebClient::FIELD, "" );
    FieldData field;
    if (!result ) {
      webClient.disconnect();
      return result;
    }
    if ( webClient.client.connected() ) {
      size_t capacity = JSON_OBJECT_SIZE(5) + 100;
      DynamicJsonBuffer jsonBuffer(capacity);

      //Serial.println( json );
      // Parse JSON object
      JsonObject& root = jsonBuffer.parseObject( webClient.client );

      if (!root.success() ) {
        Serial.print(F("Parsing Field failed!")); Serial.println( root.prettyPrintTo( Serial ));
        jsonBuffer.clear();
        webClient.disconnect();
        return false;
      }
      String str = root[F("name")];
      field.name =  str;
      field.latitude = root[F("latitude")];
      field.longitude = root[F("longitude")];
      jsonBuffer.clear();
      Serial.print(F("Field retrieved: ")); Serial.println( str );
      webClient.disconnect();
      initialised = true;
      return true;
    }
    webClient.disconnect();
    return false;
  }
}

void Field::loop() {
  Serial.println(F("Field: updating"));
  bool result = requestField();
  if (result){
    Serial.print(F("FIELD found: ")); Serial.println( field.name );
  }
}
