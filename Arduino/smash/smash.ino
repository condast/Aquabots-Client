#include <ArduinoJson.h>
#include "Interrupts.h";
#include "SDCard.h"
#include "WebClient.h"
#include "Logger.h"
#include "Encoder.h"
#include "LatLng.h"
#include "TinyGPS.h"
#include "Field.h"
#include "Garmin.h"
#include "Vessel.h"
#include "Options.h"
#include "Data.h"

#define INITIALISE F("INITIALISE")
#define MAINTENANCE F("MAINTENANCE")

//#define DEBUG 1
Interrupts interrupt2;
//SDCard sdcard;
WebClient webClient;
Logger logger;
Vessel vessel;
Garmin garmin;
Encoder encoder;
TinyGPS tinyGPS;
Field field;
Options options;
Data data;

int counter;

void setup() {
  pinMode( LED_BUILTIN, OUTPUT);
  digitalWrite( LED_BUILTIN, HIGH );

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println(F("INITIALISING..."));

  sdcard.setup();

  //Print the details
  Serial.println( F("CONFIGURATION " ));
  sdcard.print( CONFIG);
  Serial.println(); Serial.println();

  webClient.setup( CONFIG );

  field.setup();

  //Prepare Bourne encoder
  encoder.setup();
  tinyGPS.setup();
  vessel.setup();
  garmin.setup();

  interrupt2.setup();

  counter = 0;
  delay(5000);
  digitalWrite( LED_BUILTIN, LOW );
  Serial.println(F("SETUP PERFORMED SUCCESFULLY"));
}

void loop() {
  //Update every second
  int step = counter % 10;
  if ( interrupt2.getSecondsFlank() ) {
    counter++;
    switch ( step ) {
      case 0:
        break;

      case 1:
        garmin.loop();
        break;
      case 2:
        options.getOptions();
        Serial.print(F( "Logging: " ));Serial.println( String( options.isLogging() ));
        if ( options.isLogging()) {
          logger.setLogger(options.isLogging());
          encoder.log();
        }
        break;
      case 3:
        if ( options.isDebugging()) {
          encoder.debug();
        }
        break;
      case 4:
        if ( options.isLogging()) {
          logger.println( "Hello world ");
        }
        break;
      case 5:
        tinyGPS.loop();
        break;
      case 6:
        break;
      case 7:
        field.loop();
        break;
      case 8:
        //tinyGPS.sendNMEA();
        break;
      default:
        break;
    }
    vessel.loop();
    interrupt2.clear();
  }

  if ( interrupt2.getFlank() ) {
    interrupt2.clearFlank();
  }
}
