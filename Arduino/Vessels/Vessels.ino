#include <ArduinoJson.h>

#include "WebClient.h"
#include "Registration.h"
#include "TinyGPS.h"
#include "LatLng.h"
#include "Vessel.h"
#include "ServoController.h"

#define VESSEL F("AquaBoat")
#define PASSPHRASE F("AquaPassphrase")
#define LATITUDE 51.2
#define LONGITUDE 4.2
#define TIME_OUT 300 //msec

//SoftwareSerial Serial1(2, 3); // RX, TX
WebClient webClient;
Registration registration;
TinyGPS tinyGPS;
Vessel vessel;
ServoController servo;


long vesselId;

void setup() {
  Serial.begin(9600);
  Serial.print(F("Setup Vessel: ")); Serial.println( VESSEL );
  webClient.setup();
  registration.setup();
  tinyGPS.setup();
  vessel.setup();
  vesselId = -1;
}

void loop() {
  tinyGPS.loop();
  if ( vesselId < 0 ) {
    vesselId =  registration.registerVessel( VESSEL, PASSPHRASE, tinyGPS.getLatitude(), tinyGPS.getLongitude() );
    Serial.print(F("REGISTRATION FAILED: ")); Serial.println( vesselId );
  } else {
    Serial.print(F("VESSEL: ")); Serial.println( vesselId );
    vessel.loop( tinyGPS.getBearing());
  }

  delay(1000);
}
