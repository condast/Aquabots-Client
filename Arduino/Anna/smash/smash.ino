#include <ArduinoJson.h>

#include "WebClient.h"
#include "Registration.h"
#include "TinyGPS.h"
#include "LatLng.h"
#include "Vessel.h"
#include "ServoController.h"
#include "Interrupts.h"
#include "Logger.h"
#include "Options.h"

#define VESSEL F("Anna")
#define PASSPHRASE F("AquabotsRule")
#define LATITUDE 51.2
#define LONGITUDE 4.2
#define TIME_OUT 3000 //msec

static WebClient webClient;
static Registration registration;
static TinyGPS gps;
static Vessel vessel;
static Interrupts interrupt;
static Logger logger;
static Options options;
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
  gps.loop();
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