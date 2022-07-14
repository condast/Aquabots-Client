Config::Config() {};

void Config:: send() {
  String data = CONFIG_START;
  data+=ARRAY_START;
  data += gps.getConfig();
  data+=ARRAY_END;
  data += CONFIG_END;
  //Serial.print(F("READING ")); Serial.print(type); Serial.print(F(": "));
  //Serial.print(deviceName); Serial.print(F(" - "));Serial.print( sentence );
  //data += webClient.urlencode( data );
  //Serial.println( data );
 sendConfig( data );
}

/**
   Get the options
*/
bool Config::sendConfig( String msg) {
  if ( !webClient.connect()) {
    return false;
  }
  String message = F("&msg=");
    message += webClient.urlencode(msg);
  //Serial.print( "options request ("); Serial.print( request); Serial.print(F("): |" )); Serial.print( msg ); Serial.println("|");

  webClient.setContext( AQUABOTS_VESSEL_CONTEXT );
  boolean result = webClient.sendHttp( WebClient::CONFIG, false, message );
  if ( !result ) {
    //Serial.print(F("Request options failed: ")); Serial.println( msg );
    webClient.disconnect();
    return false;
  }

  size_t capacity = JSON_OBJECT_SIZE(5) + 112;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, webClient.client);
  if (error) {
    Serial.print(F("Parsing config failed!")); Serial.println( error.c_str() );
    webClient.disconnect();
    return false;
  }
  JsonObject root = doc.as<JsonObject>();
  //Serial.print(F("OPTIONS DATA " )); Serial.print( request); Serial.print(F(": " )); Serial.println( options);
  //serializeJson(doc, Serial);
  webClient.disconnect();
  return true;
}
