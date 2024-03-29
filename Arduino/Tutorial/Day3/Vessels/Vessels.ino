#include <ArduinoJson.h>

#include "WebClient.h"
#include "Registration.h"
#include "VesselGPS.h"
#include "Vessel.h"

#define VESSEL F("AquaBoat")
#define PASSPHRASE F("AquaPassphrase")
#define LATITUDE 51.2
#define LONGITUDE 4.2
#define TIME_OUT 3000 //msec

/**
 * This is the example code of Day 3
 */
 
//SoftwareSerial Serial1(2, 3); // RX, TX
static WebClient webClient;
static Registration registration;
static VesselGPS gps;
static Vessel vessel;

long vesselId;

void setup() {
  Serial.begin(9600);
  Serial.print(F("Setup Vessel: ")); Serial.println( VESSEL );
  vesselId = -1;
  webClient.setup();
  registration.setup();
  gps.setup();
  vessel.setup();
}

void loop() {
  gps.loop( true );
  if ( vesselId < 0 ) {
    vesselId =  registration.registerVessel( VESSEL, PASSPHRASE, gps.getLatitude(), gps.getLongitude() );
    if ( vesselId > 0 ) {
      Serial.print(F("REGISTERED VESSEL: ")); Serial.println( vesselId );
      webClient.setAuthentication( vesselId, PASSPHRASE );
    } else
      Serial.print(F("REGISTRATION FAILED: ")); Serial.println( vesselId );
  } else {
    Serial.print(F("VESSEL: ")); Serial.println( vesselId );
    //vessel.loop( gps.getBearing());
  }

  delay(1000);
}
