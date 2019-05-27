Data::Data() {};

void Data:: send( int request, String senderId, String senderName, String deviceName, String deviceId, String type, String sentence ) {
  if ( sentence.length() == 0)
    return;
  String data = F("&sender-id=");
  data += senderId;
  data += F("&sender-name=");
  data += webClient.urlencode(senderName);
  F("&device-id=");
  data += deviceId;
  data += F("&device-name=");
  data += webClient.urlencode(deviceName);
  data+= F("&type="); 
  data += type;
  data += F("&data=");
  data += webClient.urlencode(sentence);  
  //Serial.print(F("READING ")); Serial.print(type); Serial.print(F(": ")); 
  //Serial.print(deviceName); Serial.print(F(" - "));Serial.print( sentence );
  //data += webClient.urlencode( data );
  options.getOptions( request, data, true );
}

void Data:: sendNMEA( String deviceName, String deviceId, String sentence ) {
  Serial.print(F("\tSEND DATA:")); Serial.print(sentence); Serial.println(F("\n\n")); 
  send( WebClient::NMEA,VESSEL_ID, VESSEL, deviceName, deviceId, F("nmea"), sentence );
}
