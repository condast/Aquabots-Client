#include "Data.h"
#include "Options.h"
Data::Data() {};

void Data::send( String deviceName, String deviceId, int request, String type, String sentence ) {
  if ( sentence.length() == 0)
    return;
  String data = F("&device-name=");
  data += deviceName;
  data += F("&device-id=");
  data += deviceId;
  data+= F("&type="); 
  data += type;
  data += F("&");
  data += type;
  data += F("=");
  data += sentence;  
  //Serial.print(F("READING ")); Serial.print(type); Serial.print(F(": ")); 
  //Serial.print(deviceName); Serial.print(F(" - "));Serial.print( sentence );
  //data += webClient.urlencode( data );
  options.getOptions( request, data );
}

void Data::sendNMEA( String deviceName, String deviceId, String sentence ) {
  send( deviceName, deviceId, WebClient::NMEA, "nmea", sentence );
}
