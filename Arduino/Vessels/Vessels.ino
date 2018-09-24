#include <ArduinoJson.h>

#include "WebClient.h"
#include "SDCard.h"
#include "Registration.h"

WebClient webClient;
SDCard sdcard;
Registration registration;

void setup() {
    Serial.begin(9600);
    Serial.println("Initialised");
    webClient.setup();
    registration.setup();
}

void loop() {
  long id = registration.registerVessel( "AquaBoat", "AquaPassphrase", 51.0, 4.2 );
  Serial.print(F("Vessel id: ")); Serial.println( id );
  delay(2000);
}
