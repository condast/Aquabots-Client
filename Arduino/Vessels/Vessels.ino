#include <ArduinoJson.h>

#include "WebClient.h"
#include "Registration.h"
#include "TinyGPS.h"

#define VESSEL F("AquaBoat")
#define PASSPHRASE F("AquaPassphrase")
#define LATITUDE 51.2
#define LONGITUDE 4.2
#define TIME_OUT 300 //msec

SoftwareSerial Serial1(2, 3); // RX, TX
WebClient webClient;
Registration registration;
TinyGPS tinyGPS;

void setup() {
  Serial.begin(9600);
  Serial.print(F("Setup Vessel: ")); Serial.println( VESSEL );
  webClient.setup();
  registration.setup();
  tinyGPS.setup();
}

void loop() {
  long vesselId =  registration.registerVessel( VESSEL, PASSPHRASE, LATITUDE, LONGITUDE );
  if ( vesselId >= 0 )
    Serial.print(F("VESSEL: ")); Serial.println( vesselId );
  delay(1000);
  tinyGPS.loop();
}
