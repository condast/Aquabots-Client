#include <ArduinoJson.h>

#include "WebClient.h"
#include "SDCard.h"
#include "Registration.h"

#define VESSEL F("Costa")
#define PASSPHRASE F("HetIsWelGoed")
#define LATITUDE 51.2
#define LONGITUDE 4.2

WebClient webClient;
SDCard sdcard;
Registration registration;

void setup() {
  Serial.begin(9600);
  Serial.print(F("Setup Vessel: ")); Serial.println( VESSEL );
  webClient.setup();
  registration.setup();
}

void loop() {
  registration.registerVessel( VESSEL, PASSPHRASE, LATITUDE, LONGITUDE );
  delay(1000);
}
